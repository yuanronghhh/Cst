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

  r = sys_socket_listen(self->socket, 100);
  if (r < 0) {
    sys_error_N("listen failed: %s:%d", inet_ntoa(self->addr.sin_addr), ntohs(self->addr.sin_port));
    return false;
  }

  return true;
}

SysSSize socket_connection_connect(SocketConnection *self, const SysChar* host, const int port) {
  sys_return_val_if_fail(self != NULL, -1);

  SysSSize r = sys_socket_connect(self->socket, (struct sockaddr*)&self->addr, sizeof(struct sockaddr_in));
  if (r < 0) {
    sys_warning_N("connect remote failed: %s:%d,%s", host, port);

    return r;
  }

  return r;
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

SysSize socket_connection_handle(SocketConnection *self, SysPointer user_data) {
  sys_return_val_if_fail(self != NULL, -1);
  SysSSize r = 0;

  if(self->func) {
    r = self->func(self, user_data);
  }

  return r;
}

SysSSize socket_connection_pipe(SocketConnection* cconn, SocketConnection *rconn) {
  SysChar buffer[4096] = { 0 };
  SysSSize r = 0;
  SysSSize c = 0;

  r = sys_socket_recv(cconn->socket, buffer, sizeof(buffer), 0);
  if (r < 0) {
    return r;
  }

  c = sys_socket_send(rconn->socket, buffer, r, 0);

  return c;
}

static void socket_connection_construct(SocketConnection* self, const SysChar* host, const int port, 
  SysSocket *socket, SocketConnectionFunc func) {

  sys_return_if_fail(self != NULL);
  sys_return_if_fail(socket != NULL);

  self->addr.sin_port = htons(port);
  self->addr.sin_family = AF_INET;
  self->addr.sin_addr.s_addr = htonl(get_inet_addr(host));
  self->socket = socket;
  self->func = func;
}

SocketConnection* socket_connection_accept(SocketConnection* self, SocketConnectionFunc func) {
  sys_return_val_if_fail(self != NULL, NULL);

  SysSocket* s;
  SocketConnection* conn;
  socklen_t len = sizeof(struct sockaddr_in);
  struct sockaddr_in client_addr;

  s = sys_socket_accept(self->socket, (struct sockaddr*)&client_addr, &len);
  if (s == NULL) {
    return NULL;
  }
  sys_socket_set_noblock(s, true);

  conn = socket_connection_new_I(inet_ntoa(client_addr.sin_addr), ntohs(self->addr.sin_port), 
    s, func);
  return conn;
}

SocketConnection* socket_connection_new_I(const SysChar* host, const int port, 
  SysSocket *socket, SocketConnectionFunc func) {
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
