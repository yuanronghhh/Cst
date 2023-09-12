#include <CstDemo/FRPServer.h>

SYS_DEFINE_TYPE(FRPServer, frp_server, SYS_TYPE_OBJECT);

static void frp_set_connection(FRPServer *self, SocketConnection *conn) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(conn != NULL);

  SOCKET fd;

  fd = SCONN_SOCKET_ID(conn);
  self->maxfd = max(self->maxfd, (int)fd);

  FD_SET(fd, &self->rfds);
}

static void frp_clear_connection(FRPServer* self, SocketConnection *conn) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(conn != NULL);

  FD_CLR(SCONN_SOCKET_ID(conn), &self->rfds);
}

static SysBool frp_is_connection(FRPServer* self, SocketConnection* conn) {
  SysBool rs = FD_ISSET(SCONN_SOCKET_ID(conn), &self->rfds);

  return rs;
}

static int counter = 0;

static SysSSize frp_tunnel_connection(FRPServer * self, SocketConnection *cconn, SocketConnection *rconn) {
  sys_return_val_if_fail(self != NULL, false);

  SysSSize r;
  struct timeval tv;

  while(1) {
    counter++;

    FD_ZERO(&self->rfds);
    frp_set_connection(self, rconn);
    frp_set_connection(self, cconn);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    r = select(self->maxfd + 1, &self->rfds, NULL, NULL, &tv);
    if (r == -1 && errno != EINTR) {
      sys_warning_N("%s", sys_socket_strerror(sys_socket_errno()));
      break;
    }

    if (r == 0) {
      break;
    }

    if (FD_ISSET(SCONN_SOCKET_ID(cconn), &self->rfds)) {
      sys_debug_N("%s,%d", "client", counter);
      r = socket_connection_handle(cconn, self);

    } else if (FD_ISSET(SCONN_SOCKET_ID(rconn), &self->rfds)) {
      sys_debug_N("%s,%d", "remote", counter);
      r = socket_connection_handle(rconn, self);

    } else {

      sys_debug_N("%s,%d", "unknown", counter);
    }

    if(r <= 0) {
      break;
    }
  }

  return r;
}

SysSSize frp_handle_client(SocketConnection *self, SysPointer user_data) {
  FRPServer *server = user_data;

  SysSSize r = socket_connection_pipe(self, server->rconn);
  return r > 0 ? r : 0;
}

SysSSize frp_handle_remote(SocketConnection *self, SysPointer user_data) {
  FRPServer *server = user_data;

  SysSSize r = socket_connection_pipe(self, server->cconn);
  return r > 0 ? r : 0;
}

SysSSize frp_handle_server(SocketConnection *self, SysPointer user_data) {
  FRPServer* server = user_data;

  SocketConnection *cconn = socket_connection_accept(self, frp_handle_client);
  if (cconn == NULL) {
    sys_object_unref(self);
    return -1;
  }

  return 1; // socket_connection_pipe(cconn, server->rconn);
}

void frp_server_run(FRPServer *self) {
  sys_return_if_fail(self != NULL);

  SysSocket* s;
  SysSSize r;
  SocketConnection *rconn;
  SocketConnection *cconn;
  SocketConnection *sconn = self->server_conn;

  do {
    s = sys_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, false);
    rconn = socket_connection_new_I(self->remote_host, self->remote_port, s, frp_handle_remote);

    r = socket_connection_connect(rconn, self->remote_host, self->remote_port);
    if (r < 0) {
      sys_object_unref(rconn);
      break;
    }
    self->rconn = rconn;

    cconn = socket_connection_accept(sconn, frp_handle_client);
    if (cconn == NULL) {
      break;
    }
    self->cconn = cconn;

    r = frp_tunnel_connection(self, rconn, cconn);
    if(r == 0) {
      sys_object_unref(cconn);
      sys_object_unref(rconn);
    }
  } while (r >= 0);
}

/* object api */
static void frp_server_dispose(SysObject* o) {
  FRPServer* self = FRP_SERVER(o);

  sys_clear_pointer(&self->server_conn, (SysDestroyFunc)_sys_object_unref);

  SYS_OBJECT_CLASS(frp_server_parent_class)->dispose(o);
}

static void frp_server_construct(FRPServer* self, const int local_port, const SysChar* remote_host, const int remote_port) {
#ifndef SYS_OS_WIN32
  signal(SIGCHLD, SIG_IGN);
#endif

  self->remote_host = (SysChar*)remote_host;
  self->remote_port = remote_port;
  self->local_port = local_port;

  SSL_CTX* ssl_ctx = SSL_CTX_new(SSLv23_server_method());
  SSL* ssl = SSL_new(ssl_ctx);
  SysSocket* s;
  
#if USE_OPENSSL
  s = sys_socket_new_ssl(AF_INET, SOCK_STREAM, IPPROTO_TCP, false, ssl);
#else
  s = sys_socket_new(AF_INET, SOCK_STREAM, IPPROTO_TCP, false, );
#endif
  SocketConnection *conn = socket_connection_new_I("localhost", local_port, s, frp_handle_server);
  if(conn == NULL || !socket_connection_listen(conn)) {
    sys_abort_N("listen connect failed: %s:%d", "localhost", local_port);
    return;
  }

  self->server_conn = conn;
}

FRPServer *frp_server_new(void) {
  return sys_object_new(FRP_TYPE_SERVER, NULL);
}

FRPServer *frp_server_new_I(const int local_port, const SysChar* remote_host, const int remote_port) {
  FRPServer *o = frp_server_new();

  frp_server_construct(o, local_port, remote_host, remote_port);

  return o;
}

SysBool frp_server_setup_ssl(FRPServer *self, SSL_CTX *ssl_ctx) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(ssl_ctx != NULL, false);

  self->ssl_ctx = ssl_ctx;

  return true;
}

static void frp_server_class_init(FRPServerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = frp_server_dispose;
}

static void frp_server_init(FRPServer *self) {
}
