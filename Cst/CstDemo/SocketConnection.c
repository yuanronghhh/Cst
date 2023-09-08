#include <CstDemo/SocketConnection.h>


SYS_DEFINE_TYPE(SocketConnection, socket_connection, SYS_TYPE_OBJECT);

SysBool socket_connection_listen(SocketConnection *self) {
  int r;
  int optval = 1;

  r = sys_socket_setopt(self->socket, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval));
  if (r < 0) {

    sys_error_N("setsockopt SO_REUSEADDR failed: %d", r);
    return false;
  }

  r = sys_socket_bind(self->socket, (struct sockaddr*)&self->addr, sizeof(struct sockaddr_in));
  if (r < 0) {

    sys_error_N("bind failed: %s:%d", inet_ntoa(self->addr.sin_addr), ntohs(self->addr.sin_port));
    return false;
  }

  r = sys_socket_listen(self->socket, 1);
  if (r < 0) {
    sys_error_N("listen failed: %s:%d", inet_ntoa(self->addr.sin_addr), ntohs(self->addr.sin_port));
    return false;
  }

  return true;
}

SocketConnection *socket_connection_connect(const SysChar* host, const int port, SysSocket *socket, SocketConnectionFunc func) {
  sys_return_val_if_fail(socket != NULL, NULL);

  SocketConnection *conn = socket_connection_new_I(host, port, socket, func);
  SysSSize r = sys_socket_connect(conn->socket, (struct sockaddr*)&conn->addr, sizeof(struct sockaddr_in));
  if (r < 0) {
    sys_object_unref(conn);

    sys_warning_N("connect remote failed: %s:%d", host, port);
    return NULL;
  }

  return conn;
}

static unsigned long get_inet_addr(const SysChar* host) {
  if (sys_str_equal(host, "localhost")) {
    return INADDR_LOOPBACK;
  }

  if (sys_str_equal(host, "0.0.0.0")) {
    return INADDR_ANY;
  }

  return inet_addr(host);
}

void socket_connection_set_handle(SocketConnection *self, SocketConnectionFunc func) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(func != NULL);

  self->func = func;
}

void socket_connection_handle(SocketConnection *self, SysInt status) {
  sys_return_if_fail(self != NULL);

  if(self->func) {
    self->func(self, status);
  }

  if (status == 0) {
    sys_object_unref(self);
  }
}

static void socket_connection_construct(SocketConnection* self, const SysChar* host, const int port, SysSocket *socket, SocketConnectionFunc func) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(socket != NULL);

  self->addr.sin_port = htons(port);
  self->addr.sin_family = AF_INET;
  self->addr.sin_addr.s_addr = htonl(get_inet_addr(host));
  self->socket = socket;
  self->func = func;
}

SocketConnection* socket_connection_accept(SocketConnection* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  SysSocket* csocket;
  SocketConnection* conn;
  socklen_t len = sizeof(struct sockaddr_in);
  struct sockaddr_in client_addr;

  csocket = sys_socket_accept(self->socket, (struct sockaddr*)&client_addr, &len);
  if (csocket == NULL) {
    if (errno != EINTR) {

      sys_warning_N("accept: %d", csocket);
    }
  }

  conn = socket_connection_new_I(inet_ntoa(client_addr.sin_addr), ntohs(self->addr.sin_port), csocket);
  return conn;
}

SocketConnection* socket_connection_new_I(const SysChar* host, const int port, SysSocket *socket, SocketConnectionFunc func) {
  sys_return_val_if_fail(host != NULL, NULL);
  sys_return_val_if_fail(socket != NULL, NULL);

  SocketConnection* o = socket_connection_new();

  socket_connection_construct(o, host, port, socket, func);

  return o;
}

SysSocket* socket_connection_get_socket(SocketConnection* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->socket;
}

/* object api */
static void socket_connection_dispose(SysObject* o) {
  SocketConnection *self = SOCKET_CONNECTION(o);

  sys_clear_pointer(&self->socket, sys_socket_free);

  SYS_OBJECT_CLASS(socket_connection_parent_class)->dispose(o);
}

SocketConnection *socket_connection_new(void) {
  return sys_object_new(SOCKET_TYPE_CONNECTION, NULL);
}

static void socket_connection_class_init(SocketConnectionClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = socket_connection_dispose;
}

static void socket_connection_init(SocketConnection *self) {
}
