#include <CstDemo/FRPServer.h>


SYS_DEFINE_TYPE(FRPServer, frp_server, SYS_TYPE_OBJECT);

static void frp_set_connection(FRPServer *self, SocketConnection *conn) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(conn != NULL);

  int fd = sys_socket_get_fd(SCONN_SOCKET(conn));
  self->maxfd = max(self->maxfd, fd);

  SYS_FD_SET(SCONN_SOCKET(conn), &self->fds);
}

static void frp_clear_connection(FRPServer* self, SocketConnection *conn) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(conn != NULL);

  SYS_FD_CLR(SCONN_SOCKET(conn), &self->fds);
}

static SysBool frp_is_connection(FRPServer* self, SocketConnection* conn) {
  SysBool rs = SYS_FD_ISSET(SCONN_SOCKET(conn), &self->fds);

  return rs;
}

static SysSSize socket_connection_pipe(SocketConnection* cconn, SocketConnection *rconn) {
  SysChar buffer[8192] = { 0 };

  SysSSize r = sys_socket_recv(SCONN_SOCKET(cconn), buffer, sizeof(buffer), 0);
  if (r > 0) {
    sys_socket_send(SCONN_SOCKET(rconn), buffer, r, 0);
  }

  return r;
}

static SysSSize frp_tunnel_connection(FRPServer * self, SocketConnection *cconn, SocketConnection *rconn) {
  sys_return_val_if_fail(self != NULL, false);

  SysSSize r;
  SysSSize status = 0;
  struct timeval tv;

  tv.tv_sec = 1;
  tv.tv_usec = 0;
  fd_set *io = &self->fds;

  for (;;) {
    SYS_FD_ZERO(io);

    frp_set_connection(self, rconn);
    frp_set_connection(self, cconn);

    r = select(self->maxfd + 1, &self->fds, NULL, NULL, &tv);
    if (r < 0) {
      break;
    }

    if (frp_is_connection(self, self->server_conn)) {
      status = socket_connection_handle(self->server_conn, self);
    }

    if (frp_is_connection(self, cconn)) {
      status = socket_connection_handle(cconn, rconn);
    }

    if (frp_is_connection(self, rconn)) {
      status = socket_connection_handle(rconn, cconn);
    }

    if (status == 0) {
      break;
    }
  }

  return status;
}

SysSSize frp_handle_client(SocketConnection *self, SysPointer user_data) {
  sys_debug_N("%s", "frp_handle_client");
  SocketConnection *rconn = user_data;

  return socket_connection_pipe(self, rconn);
}

SysSSize frp_handle_remote(SocketConnection *self, SysPointer user_data) {
  sys_debug_N("%s", "frp_handle_remote");
  SocketConnection *cconn = user_data;

  return socket_connection_pipe(self, cconn);
}

SysSSize frp_handle_server(SocketConnection *self, SysPointer user_data) {
  sys_debug_N("%s", "frp_handle_server");
  FRPServer* server = user_data;

  frp_clear_connection(server, self);
  return 0;
}

void frp_server_run(FRPServer *self) {
  sys_return_if_fail(self != NULL);

  SocketConnection* rconn;
  SocketConnection* cconn;
  SysSSize r;

  while (1) {
    SysSocket* socket = sys_socket(AF_INET, SOCK_STREAM, 0);
    rconn = socket_connection_connect(self->remote_host, self->remote_port, socket, frp_handle_client);
    if (rconn == NULL) {
      break;
    }

    cconn = socket_connection_accept(self->server_conn, frp_handle_remote);
    if (cconn == NULL) {
      break;
    }

    r = frp_tunnel_connection(self, cconn, rconn);

    if(r == 0) {
      sys_object_unref(cconn);
      sys_object_unref(rconn);
    }
  }
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

  SysSocket* s = sys_socket(AF_INET, SOCK_STREAM, 0);
  SocketConnection *conn = socket_connection_new_I("localhost", local_port, s, frp_handle_server);
  if(conn == NULL || !socket_connection_listen(conn)) {
    sys_abort_N("listen connect failed: %s:%d", "localhost", local_port);
    return;
  }

  self->server_conn = conn;
  SYS_FD_ZERO(&self->fds);
  frp_set_connection(self, conn);
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
