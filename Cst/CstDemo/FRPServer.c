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

static SysSSize socket_connection_pipe(SocketConnection* cconn, SocketConnection *rconn) {
  SysChar buffer[8192] = { 0 };
  SysSSize r = 0;
  SysSSize c = 0;

  r = sys_socket_recv(socket_connection_get_socket(cconn), buffer, sizeof(buffer) - 1, 0);
  if (r < 0) {
    return r;
  }

  c = sys_socket_send(socket_connection_get_socket(rconn), buffer, r, 0);
  return c;
}

static int counter = 0;

static SysSSize frp_tunnel_connection(FRPServer * self) {
  sys_return_val_if_fail(self != NULL, false);

  SysSSize r;
  struct timeval tv;

  while(1) {
    counter++;

    FD_ZERO(&self->rfds);
    frp_set_connection(self, self->rconn);
    if (self->cconn) {

      frp_set_connection(self, self->cconn);
    } else {

      frp_set_connection(self, self->server_conn);
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    r = select(self->maxfd + 1, &self->rfds, NULL, NULL, &tv);
    if (r == -1 && errno != EINTR) {
      sys_warning_N("%s", sys_socket_strerror(sys_socket_errno()));
      break;
    }

    if (r == 0) {
      sys_clear_pointer(&self->rconn, _sys_object_unref);
      continue;
    }

    if (FD_ISSET(SCONN_SOCKET_ID(self->server_conn), &self->rfds)) {
      sys_debug_N("%s,%d", "server", counter);
      r = socket_connection_handle(self->server_conn, self);
      frp_clear_connection(self, self->server_conn);

    } else if (FD_ISSET(SCONN_SOCKET_ID(self->cconn), &self->rfds)) {
      sys_debug_N("%s,%d", "client", counter);
      r = socket_connection_handle(self->cconn, self);

    } else if (FD_ISSET(SCONN_SOCKET_ID(self->rconn), &self->rfds)) {
      sys_debug_N("%s,%d", "remote", counter);
      r = socket_connection_handle(self->rconn, self);

    } else {

      sys_debug_N("%s,%d", "unknown", counter);
    }
  }

  return r;
}

SysSSize frp_handle_client(SocketConnection *self, SysPointer user_data) {
  FRPServer *server = user_data;

  return socket_connection_pipe(self, server->rconn);
}

SysSSize frp_handle_remote(SocketConnection *self, SysPointer user_data) {
  FRPServer *server = user_data;

  return socket_connection_pipe(self, server->cconn);
}

SysSSize frp_handle_server(SocketConnection *self, SysPointer user_data) {
  FRPServer* server = user_data;

  SocketConnection *cconn = socket_connection_accept(self, frp_handle_client);
  if (cconn == NULL) {
    sys_object_unref(self);
    return -1;
  }
  server->cconn = cconn;

  return socket_connection_pipe(cconn, server->rconn);
}

void frp_server_run(FRPServer *self) {
  sys_return_if_fail(self != NULL);

  SocketConnection* rconn;
  SysSSize r;

  SysSocket* s = sys_socket(AF_INET, SOCK_STREAM, 0);
  rconn = socket_connection_connect(self->remote_host, self->remote_port, s, frp_handle_remote);
  if (rconn == NULL) {
    return;
  }
  self->rconn = rconn;

  do {
    r = frp_tunnel_connection(self);

  } while (r > 0);
}

/* object api */
static void frp_server_dispose(SysObject* o) {
  FRPServer* self = FRP_SERVER(o);

  sys_clear_pointer(&self->server_conn, (SysDestroyFunc)_sys_object_unref);
  sys_clear_pointer(&self->rconn, (SysDestroyFunc)_sys_object_unref);

  if (self->cconn) {

    sys_clear_pointer(&self->cconn, (SysDestroyFunc)_sys_object_unref);
  }

  SYS_OBJECT_CLASS(frp_server_parent_class)->dispose(o);
}

static void frp_server_construct(FRPServer* self, const int local_port, const SysChar* remote_host, const int remote_port) {
#ifndef SYS_OS_WIN32
  signal(SIGCHLD, SIG_IGN);
#endif

  self->remote_host = (SysChar*)remote_host;
  self->remote_port = remote_port;
  self->local_port = local_port;

  SysSocket* s = sys_socket(AF_INET, SOCK_STREAM, 0);

#if 0
  SysULong ul;
  sys_socket_ioctl(s, FIONBIO, &ul);
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

static void frp_server_class_init(FRPServerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = frp_server_dispose;
}

static void frp_server_init(FRPServer *self) {
}
