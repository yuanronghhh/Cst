/*
 ============================================================================
 Name        : hev-server.c
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (C) 2012 everyone.
 Description : 
 ============================================================================
 */

#include <string.h>

#include "hev-server.h"
#include "hev-protocol.h"
#include "hev-utils.h"

#ifdef G_OS_WIN32
#define MAX_THREADS    (64)
#else
#define MAX_THREADS    (16)
#endif
#define HEV_SERVER_TIMEOUT_SECONDS      600
#define HEV_SERVER_TIMEOUT_MAX_COUNT    3

#define HEV_SERVER_GET_PRIVATE(obj) (hev_server_get_instance_private(obj))

enum
{
    PROP_ZERO,
    PROP_TARGET_ADDR,
    PROP_TARGET_PORT,
    PROP_LISTEN_ADDR,
    PROP_LISTEN_PORT,
    PROP_CERT_FILE,
    PROP_KEY_FILE,
    N_PROPERTIES
};

typedef struct _HevServerPrivate HevServerPrivate;
typedef struct _HevServerSession HevServerSession;

struct _HevServerPrivate
{
    gchar *target_addr;
    gint target_port;
    gchar *listen_addr;
    gint listen_port;
    gchar *cert_file;
    gchar *key_file;

    GSocketService *service;
    HevTaskThreadPool *stpool;
    GSList *session_list;
    gboolean use_tls;

    guint timeout_id;
};

struct _HevServerSession
{
    GIOStream *tun_stream;
    GIOStream *tgt_stream;

    HevServer *self;
    GSocketClient *client;
    GMainContext *context;
    GCancellable *cancellable;
    gsize task_thread_idx;
    guint timeout_count;
    guint8 res_buffer[HEV_PROTO_HTTP_RESPONSE_VALID_LENGTH +
        HEV_PROTO_HEADER_MAXN_SIZE];
};

static void hev_server_async_initable_iface_init (GAsyncInitableIface *iface);
static void hev_server_async_initable_init_async (GAsyncInitable *initable,
            gint io_priority, GCancellable *cancellable,
            GAsyncReadyCallback callback, gpointer user_data);
static gboolean hev_server_async_initable_init_finish (GAsyncInitable *initable,
            GAsyncResult *result, GError **error);
static void pollable_splice_preread_handler (GIOStream *stream,
            gpointer data, gsize size, gpointer *buffer, gssize *len,
            gpointer user_data);
static void pollable_splice_prewrite_handler (GIOStream *stream,
            gpointer data, gsize size, gpointer *buffer, gssize *len,
            gpointer user_data);
static void session_list_free_handler (gpointer data);
static void session_list_foreach_handler (gpointer data, gpointer user_data);
static gboolean timeout_handler (gpointer user_data);
static gboolean socket_service_incoming_handler (GSocketService *service,
            GSocketConnection *connection, GObject *source_object,
            gpointer user_data);
static gboolean attach_task_handler (gpointer user_data);
static void tls_connection_handshake_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static void buffered_input_stream_fill_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static void invalid_output_stream_write_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static void input_stream_skip_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static void valid_output_stream_write_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static void socket_client_connect_to_host_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static void io_stream_splice_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static void io_stream_close_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data);
static gboolean detach_task_handler (gpointer user_data);

static GParamSpec *hev_server_properties[N_PROPERTIES] = { NULL };

G_DEFINE_TYPE_WITH_CODE (HevServer, hev_server, G_TYPE_OBJECT,
        G_ADD_PRIVATE (HevServer); G_IMPLEMENT_INTERFACE (G_TYPE_ASYNC_INITABLE, hev_server_async_initable_iface_init));

GQuark
hev_server_error_quark (void)
{
    return g_quark_from_static_string ("hev-server-error-quark");
}

static void
hev_server_dispose (GObject *obj)
{
    HevServer *self = HEV_SERVER (obj);
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    if (priv->timeout_id) {
        g_source_remove (priv->timeout_id);
        priv->timeout_id = 0;
    }

    if (priv->service) {
        g_signal_handlers_disconnect_by_func (priv->service,
                    G_CALLBACK (socket_service_incoming_handler),
                    self);
        g_object_unref (priv->service);
        priv->service = NULL;
    }

    if (priv->stpool) {
        hev_task_thread_pool_free (priv->stpool);
        priv->stpool = NULL;
    }

    if (priv->session_list) {
        g_slist_free_full (priv->session_list,
                    session_list_free_handler);
        priv->session_list = NULL;
    }

    G_OBJECT_CLASS (hev_server_parent_class)->dispose (obj);
}

static void
hev_server_finalize (GObject *obj)
{
    HevServer *self = HEV_SERVER (obj);
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    if (priv->target_addr) {
        g_free (priv->target_addr);
        priv->target_addr = NULL;
    }

    if (priv->listen_addr) {
        g_free (priv->listen_addr);
        priv->listen_addr = NULL;
    }

    if (priv->cert_file) {
        g_free (priv->cert_file);
        priv->cert_file = NULL;
    }

    if (priv->key_file) {
        g_free (priv->key_file);
        priv->key_file = NULL;
    }

    G_OBJECT_CLASS (hev_server_parent_class)->finalize (obj);
}

static GObject *
hev_server_constructor (GType type,
            guint n,
            GObjectConstructParam *param)
{
    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    return G_OBJECT_CLASS (hev_server_parent_class)->constructor (type, n, param);
}

static void
hev_server_constructed (GObject *obj)
{
    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    G_OBJECT_CLASS (hev_server_parent_class)->constructed (obj);
}

static void
hev_server_get_property (GObject *obj, guint id,
            GValue *value, GParamSpec *pspec)
{
    HevServer *self = HEV_SERVER (obj);
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    switch (id) {
    case PROP_TARGET_ADDR:
        g_value_set_string (value, priv->target_addr);
        break;
    case PROP_TARGET_PORT:
        g_value_set_int (value, priv->target_port);
        break;
    case PROP_LISTEN_ADDR:
        g_value_set_string (value, priv->listen_addr);
        break;
    case PROP_LISTEN_PORT:
        g_value_set_int (value, priv->listen_port);
        break;
    case PROP_CERT_FILE:
        g_value_set_string (value, priv->cert_file);
        break;
    case PROP_KEY_FILE:
        g_value_set_string (value, priv->key_file);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, id, pspec);
        break;
    }
}

static void
hev_server_set_property (GObject *obj, guint id,
            const GValue *value, GParamSpec *pspec)
{
    HevServer *self = HEV_SERVER (obj);
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    switch (id) {
    case PROP_TARGET_ADDR:
        if (priv->target_addr)
          g_free (priv->target_addr);
        priv->target_addr = g_strdup (g_value_get_string (value));
        break;
    case PROP_TARGET_PORT:
        priv->target_port = g_value_get_int (value);
        break;
    case PROP_LISTEN_ADDR:
        if (priv->listen_addr)
          g_free (priv->listen_addr);
        priv->listen_addr = g_strdup (g_value_get_string (value));
        break;
    case PROP_LISTEN_PORT:
        priv->listen_port = g_value_get_int (value);
        break;
    case PROP_CERT_FILE:
        if (priv->cert_file)
          g_free (priv->cert_file);
        priv->cert_file = g_strdup (g_value_get_string (value));
        break;
    case PROP_KEY_FILE:
        if (priv->key_file)
          g_free (priv->key_file);
        priv->key_file = g_strdup (g_value_get_string (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, id, pspec);
        break;
    }
}

static void
hev_server_class_init (HevServerClass *klass)
{
    GObjectClass *obj_class = G_OBJECT_CLASS (klass);

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    obj_class->constructor = hev_server_constructor;
    obj_class->constructed = hev_server_constructed;
    obj_class->set_property = hev_server_set_property;
    obj_class->get_property = hev_server_get_property;
    obj_class->dispose = hev_server_dispose;
    obj_class->finalize = hev_server_finalize;

    /* Properties */
    hev_server_properties[PROP_TARGET_ADDR] =
        g_param_spec_string ("target-addr",
                    "target addr", "Target addr",
                    "127.0.0.1",
                    G_PARAM_READWRITE |
                    G_PARAM_CONSTRUCT_ONLY);
    hev_server_properties[PROP_TARGET_PORT] =
        g_param_spec_int ("target-port",
                    "target port", "Target port",
                    0, G_MAXUINT16, 22,
                    G_PARAM_READWRITE |
                    G_PARAM_CONSTRUCT_ONLY);
    hev_server_properties[PROP_LISTEN_ADDR] =
        g_param_spec_string ("listen-addr",
                    "listen addr", "Listen addr",
                    "0.0.0.0",
                    G_PARAM_READWRITE |
                    G_PARAM_CONSTRUCT_ONLY);
    hev_server_properties[PROP_LISTEN_PORT] =
        g_param_spec_int ("listen-port",
                    "listen port", "Listen port",
                    0, G_MAXUINT16, 6000,
                    G_PARAM_READWRITE |
                    G_PARAM_CONSTRUCT_ONLY);
    hev_server_properties[PROP_CERT_FILE] =
        g_param_spec_string ("cert-file",
                    "certificate file", "Certificate file",
                    NULL,
                    G_PARAM_READWRITE |
                    G_PARAM_CONSTRUCT_ONLY);
    hev_server_properties[PROP_KEY_FILE] =
        g_param_spec_string ("key-file",
                    "private key file", "Private key file",
                    NULL,
                    G_PARAM_READWRITE |
                    G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_properties (obj_class, N_PROPERTIES,
                hev_server_properties);
}

static void
hev_server_async_initable_iface_init (GAsyncInitableIface *iface)
{
    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    iface->init_async = hev_server_async_initable_init_async;
    iface->init_finish = hev_server_async_initable_init_finish;
}

static void
hev_server_init (HevServer *self)
{
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    priv->target_addr = g_strdup ("127.0.0.1");
    priv->target_port = 22;
    priv->listen_addr = g_strdup ("0.0.0.0");
    priv->listen_port = 6000;
    priv->cert_file = NULL;
    priv->key_file = NULL;
}

static void
async_result_run_in_thread_handler (GSimpleAsyncResult *simple,
            GObject *object, GCancellable *cancellable)
{
    HevServer *self = HEV_SERVER (object);
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);
    GInetAddress *iaddr = NULL;
    GSocketAddress *saddr = NULL;
    GError *error = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    priv->service = g_socket_service_new ();
    if (!priv->service) {
        g_simple_async_result_set_error (simple,
                    HEV_SERVER_ERROR,
                    HEV_SERVER_ERROR_SERVICE,
                    "Create socket service failed!");
        goto service_fail;
    }
    g_object_set (priv->service, "listen-backlog", 100, NULL);

    iaddr = g_inet_address_new_from_string (priv->listen_addr);
    saddr = g_inet_socket_address_new (iaddr, priv->listen_port);

    if (!g_socket_listener_add_address (G_SOCKET_LISTENER (priv->service),
                saddr, G_SOCKET_TYPE_STREAM,
                G_SOCKET_PROTOCOL_TCP, NULL,
                NULL, &error)) {
        g_simple_async_result_take_error (simple, error);
        goto add_addr_fail;
    }

    g_object_unref (saddr);
    g_object_unref (iaddr);

    g_signal_connect (priv->service, "incoming",
                G_CALLBACK (socket_service_incoming_handler),
                self);

    priv->stpool = hev_task_thread_pool_new (MAX_THREADS);
    if (!priv->stpool) {
        g_simple_async_result_set_error (simple,
                    HEV_SERVER_ERROR,
                    HEV_SERVER_ERROR_CLIENT,
                    "Create splice thread pool failed!");
        goto stpool_fail;
    }

    priv->use_tls = !(g_str_equal ("None", priv->cert_file) |
        g_str_equal ("None", priv->key_file));

    priv->timeout_id = g_timeout_add_seconds (
                HEV_SERVER_TIMEOUT_SECONDS,
                timeout_handler, self);

    return;

stpool_fail:
add_addr_fail:
    g_object_unref (saddr);
    g_object_unref (iaddr);
    g_object_unref (priv->service);
service_fail:

    return;
}

static void
hev_server_async_initable_init_async (GAsyncInitable *initable,
            gint io_priority, GCancellable *cancellable,
            GAsyncReadyCallback callback, gpointer user_data)
{
    GSimpleAsyncResult *simple = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);
    
    simple = g_simple_async_result_new (G_OBJECT(initable),
                callback, user_data, hev_server_async_initable_init_async);
    g_simple_async_result_set_check_cancellable (simple, cancellable);
    g_simple_async_result_run_in_thread (simple, async_result_run_in_thread_handler,
                io_priority, cancellable);
    g_object_unref (simple);
}

static gboolean
hev_server_async_initable_init_finish (GAsyncInitable *initable,
            GAsyncResult *result, GError **error)
{
    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_return_val_if_fail (g_simple_async_result_is_valid (result,
                    G_OBJECT (initable), hev_server_async_initable_init_async),
                FALSE);
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (result),
                    error))
      return FALSE;

    return TRUE;
}

void
hev_server_new_async (gchar *target_addr, gint target_port,
            gchar *listen_addr, gint listen_port,
            gchar *cert_file, gchar *key_file,
            GCancellable *cancellable, GAsyncReadyCallback callback,
            gpointer user_data)
{
    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_async_initable_new_async (HEV_TYPE_SERVER, G_PRIORITY_DEFAULT,
                cancellable, callback, user_data,
                "target-addr", target_addr,
                "target-port", target_port,
                "listen-addr", listen_addr,
                "listen-port", listen_port,
                "cert-file", cert_file,
                "key-file", key_file,
                NULL);
}

HevServer *
hev_server_new_finish (GAsyncResult *res, GError **error)
{
    GObject *object;
    GObject *source_object;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    source_object = g_async_result_get_source_object (res);
    g_assert (NULL != source_object);

    object = g_async_initable_new_finish (G_ASYNC_INITABLE (source_object),
                res, error);

    g_object_unref (source_object);

    if (NULL != object)
      return HEV_SERVER (object);

    return NULL;
}

void
hev_server_start (HevServer *self)
{
    HevServerPrivate *priv = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_return_if_fail (HEV_IS_SERVER (self));
    priv = HEV_SERVER_GET_PRIVATE (self);

    g_socket_service_start (priv->service);
}

void
hev_server_stop (HevServer *self)
{
    HevServerPrivate *priv = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_return_if_fail (HEV_IS_SERVER (self));
    priv = HEV_SERVER_GET_PRIVATE (self);

    g_socket_service_stop (priv->service);
}

static void
pollable_splice_preread_handler (GIOStream *stream,
            gpointer data, gsize size, gpointer *buffer, gssize *len,
            gpointer user_data)
{
    HevServerSession *session = user_data;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    *buffer = data;
    *len = size;

    session->timeout_count = 0;
}

static void
pollable_splice_prewrite_handler (GIOStream *stream,
            gpointer data, gsize size, gpointer *buffer, gssize *len,
            gpointer user_data)
{
    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    hev_bytes_xor (data, size, hev_protocol_get_xor_byte ());

    *buffer = data;
    *len = size;
}

static void
session_list_free_handler (gpointer data)
{
    HevServerSession *session = data;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);
    g_io_stream_close_async (session->tgt_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);
}

static void
session_list_foreach_handler (gpointer data, gpointer user_data)
{
    HevServerSession *session = data;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    if (HEV_SERVER_TIMEOUT_MAX_COUNT < session->timeout_count) {
        if (g_cancellable_is_cancelled (session->cancellable)) {
            if (session->tun_stream) {
                g_io_stream_close_async (session->tun_stream,
                            G_PRIORITY_DEFAULT, NULL,
                            io_stream_close_async_handler,
                            session);
            }
            if (session->tgt_stream) {
                g_io_stream_close_async (session->tgt_stream,
                            G_PRIORITY_DEFAULT, NULL,
                            io_stream_close_async_handler,
                            session);
            }
        } else {
            g_cancellable_cancel (session->cancellable);
        }
    }

    session->timeout_count ++;

    return;
}

static gboolean
timeout_handler (gpointer user_data)
{
    HevServer *self = HEV_SERVER (user_data);
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_slist_foreach (priv->session_list,
                session_list_foreach_handler, self);

    return TRUE;
}

static gboolean
socket_service_incoming_handler (GSocketService *service,
            GSocketConnection *connection,
            GObject *source_object,
            gpointer user_data)
{
    HevServer *self = HEV_SERVER (user_data);
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (self);
    GTlsCertificate *cert = NULL;
    GIOStream *tun_stream = NULL;
    HevServerSession *session = NULL;
    GError *error = NULL;
    GSource *source;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    if (priv->use_tls) {
        cert = g_tls_certificate_new_from_files (priv->cert_file,
                    priv->key_file, &error);
        if (!cert) {
            g_critical ("Create TLS certificate failed: %s",
                        error ? error->message : NULL);
            g_clear_error (&error);
            goto cert_fail;
        }

        tun_stream = g_tls_server_connection_new (G_IO_STREAM (connection),
                    cert, &error);
        if (!tun_stream) {
            g_critical ("Create TLS server connection failed: %s",
                        error ? error->message : NULL);
            g_clear_error (&error);
            goto tun_stream_fail;
        }

        g_object_unref (cert);
    } else {
        tun_stream = g_object_ref (connection);
    }

    session = g_slice_new0 (HevServerSession);
    if (!session) {
        g_critical ("Alloc client data failed!");
        goto session_fail;
    }
    session->tun_stream = tun_stream;
    session->cancellable = g_cancellable_new ();
    session->self = g_object_ref (self);

    priv->session_list = g_slist_append (priv->session_list,
                session);

    session->context = hev_task_thread_pool_request (priv->stpool,
                &session->task_thread_idx);
    source = g_idle_source_new ();
    g_source_set_callback (source, attach_task_handler, session, NULL);
    g_source_attach (source, session->context);
    g_source_unref (source);

    return FALSE;

session_fail:
    g_object_unref (tun_stream);
tun_stream_fail:
    g_object_unref (cert);
cert_fail:

    return FALSE;
}

static gboolean
attach_task_handler (gpointer user_data)
{
    HevServerSession *session = user_data;
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (session->self);

    session->client = g_socket_client_new ();
    if (!session->client)
      goto client_fail;

    if (priv->use_tls) {
        g_tls_connection_handshake_async (G_TLS_CONNECTION (session->tun_stream),
                    G_PRIORITY_DEFAULT, session->cancellable,
                    tls_connection_handshake_async_handler,
                    session);
    } else {
        GInputStream *tun_input = NULL;
        GInputStream *tun_bufed_input = NULL;

        tun_input = g_io_stream_get_input_stream (session->tun_stream);
        tun_bufed_input = g_buffered_input_stream_new_sized (tun_input,
                    HEV_PROTO_HTTP_REQUEST_LENGTH + HEV_PROTO_HEADER_REAL_SIZE);
        g_filter_input_stream_set_close_base_stream (
                    G_FILTER_INPUT_STREAM (tun_bufed_input),
                    FALSE);
        g_buffered_input_stream_fill_async (
                    G_BUFFERED_INPUT_STREAM (tun_bufed_input),
                    HEV_PROTO_HTTP_REQUEST_LENGTH + HEV_PROTO_HEADER_REAL_SIZE,
                    G_PRIORITY_DEFAULT, session->cancellable,
                    buffered_input_stream_fill_async_handler,
                    session);
    }

    return G_SOURCE_REMOVE;

client_fail:
    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);

    return G_SOURCE_REMOVE;
}

static void
tls_connection_handshake_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;
    GInputStream *tun_input = NULL;
    GInputStream *tun_bufed_input = NULL;
    GError *error = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    if (!g_tls_connection_handshake_finish (G_TLS_CONNECTION (source_object),
                    res, &error)) {
        g_critical ("TLS connection handshake failed: %s", error ? error->message : NULL);
        g_clear_error (&error);
        goto handshake_fail;
    }

    tun_input = g_io_stream_get_input_stream (session->tun_stream);
    tun_bufed_input = g_buffered_input_stream_new_sized (tun_input,
                HEV_PROTO_HTTP_REQUEST_LENGTH + HEV_PROTO_HEADER_REAL_SIZE);
    g_filter_input_stream_set_close_base_stream (
                G_FILTER_INPUT_STREAM (tun_bufed_input),
                FALSE);
    g_buffered_input_stream_fill_async (
                G_BUFFERED_INPUT_STREAM (tun_bufed_input),
                HEV_PROTO_HTTP_REQUEST_LENGTH + HEV_PROTO_HEADER_REAL_SIZE,
                G_PRIORITY_DEFAULT, session->cancellable,
                buffered_input_stream_fill_async_handler,
                session);

    return;

handshake_fail:
    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);

    return;
}

static void
buffered_input_stream_fill_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;
    GBufferedInputStream *tun_bufed_input = NULL;
    gssize size = 0;
    gconstpointer req_buffer;
    const HevProtocolHeader *proto_header = NULL;
    gsize len = 0;
    GError *error = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    tun_bufed_input = G_BUFFERED_INPUT_STREAM (source_object);
    size = g_buffered_input_stream_fill_finish (tun_bufed_input,
                res, &error);
    switch (size) {
    case -1:
        g_critical ("Buffered input stream fill failed: %s",
                    error ? error->message : NULL);
        g_clear_error (&error);
    case 0:
        goto closed;
    }

    req_buffer = g_buffered_input_stream_peek_buffer (tun_bufed_input, &len);
    proto_header = (HevProtocolHeader *)((const char *)req_buffer + HEV_PROTO_HTTP_REQUEST_LENGTH);
    if (hev_protocol_header_is_valid (proto_header)) {
        GInputStream *tun_input = NULL;

        tun_input = g_filter_input_stream_get_base_stream (
                    G_FILTER_INPUT_STREAM (tun_bufed_input));
        g_input_stream_skip_async (tun_input,
                    proto_header->length - HEV_PROTO_HEADER_REAL_SIZE,
                    G_PRIORITY_DEFAULT, NULL, 
                    input_stream_skip_async_handler,
                    session);
    } else {
        GOutputStream *tun_output = NULL;
        const gchar *buffer = NULL;
        gsize count = 0;

        tun_output = g_io_stream_get_output_stream (session->tun_stream);
        buffer = hev_protocol_get_invalid_message (&count);
        g_output_stream_write_async (tun_output, buffer, count,
                    G_PRIORITY_DEFAULT, session->cancellable,
                    invalid_output_stream_write_async_handler,
                    session);
    }

    g_object_unref (tun_bufed_input);

    return;

closed:
    g_object_unref (tun_bufed_input);
    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);

    return;
}

static void
invalid_output_stream_write_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_output_stream_write_finish (G_OUTPUT_STREAM (source_object),
                res, NULL);

    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);
}

static void
input_stream_skip_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;
    gssize size = 0;
    GOutputStream *tun_output = NULL;
    HevProtocolHeader *proto_header = NULL;
    guint32 length = 0;
    GError *error = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    size = g_input_stream_skip_finish (G_INPUT_STREAM (source_object),
                res, &error);
    switch (size) {
    case -1:
        g_critical ("Buffered input stream fill failed: %s",
                    error ? error->message : NULL);
        g_clear_error (&error);
    case 0:
        goto closed;
    }

    memcpy (session->res_buffer, HEV_PROTO_HTTP_RESPONSE_VALID,
                HEV_PROTO_HTTP_RESPONSE_VALID_LENGTH);
    proto_header = (HevProtocolHeader *)(session->res_buffer +
                HEV_PROTO_HTTP_RESPONSE_VALID_LENGTH);
    length = g_random_int_range (HEV_PROTO_HEADER_MINN_SIZE,
                HEV_PROTO_HEADER_MAXN_SIZE);
    hev_protocol_header_set (proto_header, length);
    tun_output = g_io_stream_get_output_stream (session->tun_stream);
    g_output_stream_write_async (tun_output, session->res_buffer,
                HEV_PROTO_HTTP_RESPONSE_VALID_LENGTH + length,
                G_PRIORITY_DEFAULT, NULL,
                valid_output_stream_write_async_handler,
                session);

    return;

closed:
    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);

    return;
}

static void
valid_output_stream_write_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (session->self);
    gssize size = 0;
    GError *error = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    size = g_output_stream_write_finish (G_OUTPUT_STREAM (source_object),
                res, &error);
    switch (size) {
    case -1:
        g_critical ("Tunnel connection write failed: %s", error ? error->message : NULL);
        g_clear_error (&error);
    case 0:
        goto closed;
    }

    g_socket_client_connect_to_host_async (session->client,
                priv->target_addr, priv->target_port, session->cancellable,
                socket_client_connect_to_host_async_handler,
                session);

    return;

closed:
    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);

    return;
}

static void
socket_client_connect_to_host_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (session->self);
    HevPollableIOStreamSplicePrewriteFunc prewrite_handler;
    GSocketConnection *conn = NULL;
    GError *error = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    conn = g_socket_client_connect_to_host_finish (
                G_SOCKET_CLIENT (source_object), res, &error);
    if (!conn) {
        g_critical ("Connect to target host failed: %s", error ? error->message : NULL);
        g_clear_error (&error);
        goto connect_fail;
    }
    session->tgt_stream = G_IO_STREAM (conn);

    prewrite_handler = priv->use_tls ? NULL : pollable_splice_prewrite_handler;
    hev_pollable_io_stream_splice_async (session->tgt_stream,
                session->tun_stream, G_PRIORITY_DEFAULT, session->context,
                pollable_splice_preread_handler, prewrite_handler, session,
                session->cancellable, io_stream_splice_async_handler, session);

    return;

connect_fail:
    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);

    return;
}

static void
io_stream_splice_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (session->self);
    GError *error = NULL;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    if (!hev_pollable_io_stream_splice_finish (res, &error)) {
        g_debug ("Splice tunnel and target stream failed: %s", error ? error->message : NULL);
        g_clear_error (&error);
    }
    hev_task_thread_pool_release (priv->stpool, session->task_thread_idx);

    g_io_stream_close_async (session->tun_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);
    g_io_stream_close_async (session->tgt_stream,
                G_PRIORITY_DEFAULT, NULL,
                io_stream_close_async_handler,
                session);
}

static void
io_stream_close_async_handler (GObject *source_object,
            GAsyncResult *res, gpointer user_data)
{
    HevServerSession *session = user_data;

    g_debug ("%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__);

    g_io_stream_close_finish (G_IO_STREAM (source_object),
                res, NULL);

    if (G_IO_STREAM (source_object) == session->tun_stream)
      session->tun_stream = NULL;
    else if (G_IO_STREAM (source_object) == session->tgt_stream)
      session->tgt_stream = NULL;
    g_object_unref (source_object);

    if (!session->tun_stream && !session->tgt_stream)
      g_idle_add (detach_task_handler, session);
}

static gboolean
detach_task_handler (gpointer user_data)
{
    HevServerSession *session = user_data;
    HevServerPrivate *priv = HEV_SERVER_GET_PRIVATE (session->self);

    g_object_unref (session->cancellable);
    g_object_unref (session->self);
    g_object_unref (session->client);
    g_slice_free (HevServerSession, session);

    priv->session_list = g_slist_remove (priv->session_list,
                session);

    return G_SOURCE_REMOVE;
}

