#include <CstDemo/FrpServer.h>


SYS_DEFINE_TYPE(FRPServer, frp_server, SYS_TYPE_OBJECT);

static void frp_add_connection(FRPServer *self, SocketConnection *conn) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(conn != NULL);

  int fd = sys_socket_get_fd(SCONN_SOCKET(conn));
  self->maxfd = max(self->maxfd, fd);

  SYS_FD_SET(SCONN_SOCKET(conn), &self->fds);
}

static void  frp_remove_connection(FRPServer* self, SocketConnection* conn) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(conn != NULL);

  SYS_FD_CLR(SCONN_SOCKET(conn), &self->fds);
}

static SysBool frp_is_connection(FRPServer* self, SocketConnection* conn) {
  SysBool rs = SYS_FD_ISSET(SCONN_SOCKET(conn), &self->fds);

  return rs;
}

static SysSSize frp_pipe_connection(FRPServer* self, SocketConnection* cconn, SocketConnection *rconn) {
  SysChar buffer[8192] = { 0 };

  SysSSize r = sys_socket_recv(SCONN_SOCKET(cconn), buffer, sizeof(buffer), 0);
  if (r > 0) {
    sys_socket_send(SCONN_SOCKET(rconn), buffer, r, 0);
  }

  return r;
}

static int frp_poll(FRPServer* self) {
  struct timeval tv;
  int r;

  tv.tv_sec = 1;
  tv.tv_usec = 0;

  r = select(self->maxfd + 1, &self->fds, NULL, NULL, &tv);

  return r;
}

static SysBool frp_check_return(SysSSize r, SocketConnection *conn) {
  if(r < 0) {
    return false;
  }

  if (r == 0) {
    sys_object_unref(conn);
    return false;
  }

  return true;
}

static SysInt frp_tunnel_connection(FRPServer * self, SocketConnection *cconn, SocketConnection *rconn) {
  sys_return_val_if_fail(self != NULL, false);

  fd_set *io = &self->fds;
  SysInt r;
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  SYS_FD_ZERO(io);
  frp_add_connection(self, self->server_conn);

  for (;;) {
    SYS_FD_ZERO(io);

    frp_add_connection(self, rconn);
    frp_add_connection(self, cconn);

    r = select(self->maxfd + 1, &self->fds, NULL, NULL, &tv);
    if (r < 0) {
      break;
    }

    if (frp_is_connection(self, self->server_conn)) {
      sys_debug_N("server_conn: %d", r);

      if (r == 1) {
        frp_remove_connection(self, self->server_conn);
        continue;
      }
    }

    if (frp_is_connection(self, rconn)) {
      sys_debug_N("rconn: %d", r);
      SysSSize nr = frp_pipe_connection(self, rconn, cconn);

      if(!frp_check_return(nr, rconn)) {
        break;
      }
    }

    if(frp_is_connection(self, cconn)) {
      sys_debug_N("cconn: %d", r);
      SysSSize nr = frp_pipe_connection(self, cconn, rconn);
      if(!frp_check_return(nr, rconn)) {
        break;
      }
      break;
    }
  }

  return r;
}

void frp_server_run(FRPServer *self) {
  sys_return_if_fail(self != NULL);

  SocketConnection* rconn;
  SocketConnection* cconn;
  SysInt r;

  while (1) {
    SysSocket* s = sys_socket(AF_INET, SOCK_STREAM, 0);
    rconn = socket_connection_connect(self->remote_host, self->remote_port, s);
    if (rconn == NULL) {
      break;
    }

    cconn = socket_connection_accept(self->server_conn);
    if (cconn == NULL) {
      break;
    }

    r = frp_tunnel_connection(self, cconn, rconn);
    break;
  }
}

/* object api */
static void frp_server_dispose(SysObject* o) {
  FRPServer* self = FRP_SERVER(o);

  sys_clear_pointer(&self->server_conn, _sys_object_unref);

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
  SocketConnection *conn = socket_connection_new_I("localhost", local_port, s);
  if(conn == NULL || !socket_connection_listen(conn)) {
    sys_abort_N("listen connect failed: %s:%d", "localhost", local_port);
    return;
  }

  self->server_conn = conn;

  SYS_FD_ZERO(&self->fds);
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
