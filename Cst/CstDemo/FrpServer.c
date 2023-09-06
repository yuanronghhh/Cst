#include <CstDemo/FrpServer.h>

#define BUFF_SIZE 8192

SYS_DEFINE_TYPE(FRPServer, frp_server, SYS_TYPE_OBJECT);

static void frp_close_connection(FRPServer *self) {
  sys_clear_pointer(&self->client_socket, sys_socket_free);
  sys_clear_pointer(&self->remote_socket, sys_socket_free);
}

static SysBool frp_create_server(FRPServer *self) {
  int r;
  int optval = 1;

  memset(&self->server_addr, 0, sizeof(self->server_addr));

  self->server_addr.sin_port = htons(self->local_port);
  self->server_addr.sin_family = AF_INET;
  self->server_addr.sin_addr.s_addr = INADDR_ANY;

  self->server_socket = sys_socket(AF_INET, SOCK_STREAM, 0);
  if (self->server_socket == NULL) {

    sys_error_N("create failed on port: %d", self->local_port);
    return false;
  }

  r = sys_socket_setopt(self->server_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval));
  if (r < 0) {

    sys_error_N("setsockopt: %d", r);
    return false;
  }

  r = sys_socket_bind(self->server_socket, (struct sockaddr*)&self->server_addr, sizeof(self->server_addr));
  if (r < 0) {

    sys_error_N("bind: %d", r);
    return false;
  }

  r = sys_socket_listen(self->server_socket, 1);
  if (r < 0) {
    sys_error_N("listen: %d", r);
    return false;
  }

  return true;
}

static SysBool frp_accept_clients(FRPServer* self) {
  socklen_t client_addr_size = sizeof(struct sockaddr_in);

  self->client_socket = sys_socket_accept(self->server_socket, (struct sockaddr*)&self->client_addr, &client_addr_size);
  if (self->client_socket == NULL) {
    if (errno != EINTR) {

      sys_warning_N("accept: %d", self->client_socket);
    }
  }

  return true;
}

static SysBool frp_build_tunnel(FRPServer *self) {
  SysSSize r;
  if (self->remote_host == NULL) {
    sys_error_N("%s", "remote_host is NULL");
    return false;
  }

  memset(&self->remote_addr, 0, sizeof(self->remote_addr));

  self->remote_addr.sin_family = AF_INET;
  self->remote_addr.sin_port = htons(self->remote_port);

  memcpy(&self->remote_addr.sin_addr.s_addr, self->remote_host->h_addr, self->remote_host->h_length);

  self->remote_socket = sys_socket(AF_INET, SOCK_STREAM, 0);
  if (self->remote_socket < 0) {

    sys_warning_N("socket: %d", self->remote_socket);
    return false;
  }

  r = sys_socket_connect(self->remote_socket, (struct sockaddr*)&self->remote_addr, sizeof(self->remote_addr));
  if (r < 0) {
    sys_warning_N("connect remote failed: %s:%d", self->remote_host->h_name, self->remote_port);

    frp_close_connection(self);
    return false;
  }

  return true;
}

static int frp_get_fd(FRPServer* self) {
  SysInt fd = max(sys_socket_get_fd(self->client_socket), sys_socket_get_fd(self->remote_socket));
  return (int)(fd + 1);
}

static SysBool frp_handle_tunnel(FRPServer* self) {
  fd_set io;
  struct timeval tv;
  SysSSize r;
  SysChar buffer[BUFF_SIZE] = { 0 };

  tv.tv_sec = 10;
  tv.tv_usec = 0;

  for (;;) {
    SYS_FD_ZERO(&io);

    SYS_FD_SET(self->remote_socket, &io);
    SYS_FD_SET(self->client_socket, &io);
    SYS_FD_SET(self->server_socket, &io);

    int nfds = frp_get_fd(self);
    r = select(nfds, &io, NULL, NULL, &tv);

    if (r < 0) {
      goto fail;
    }

    if (SYS_FD_ISSET(self->client_socket, &io)) {
      r = sys_socket_recv(self->client_socket, buffer, sizeof(buffer), 0);
      if (r < 0) {

        sys_warning_N("recv(client_socket): %d", r);
        goto fail;
      }

      if (r == 0) {
        break;
      }

      sys_socket_send(self->remote_socket, buffer, r, 0);

    } else if (SYS_FD_ISSET(self->remote_socket, &io)) {
      r = sys_socket_recv(self->remote_socket, buffer, sizeof(buffer), 0);
      if (r < 0) {
        sys_warning_N("recv(remote_socket): %d", r);
        goto fail;
      }

      if (r == 0) {
        break;
      }

      sys_socket_send(self->client_socket, buffer, r, 0);

    } else if (SYS_FD_ISSET(self->server_socket, &io)) {

      sys_socket_send(self->client_socket, buffer, r, 0);

    } else {

      // sys_error_N("Not found fd type: %d", nfds);
      goto fail;
    }
  }

  frp_close_connection(self);
  return true;

fail:
  frp_close_connection(self);
  return false;
}

void frp_server_run(FRPServer *s) {
  while(frp_accept_clients(s)) {

    if (!frp_build_tunnel(s)) {
      continue;
    }

    frp_handle_tunnel(s);
  }

  sys_clear_pointer(&s->server_socket, sys_socket_free);
}

/* object api */
static void frp_server_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(frp_server_parent_class)->dispose(o);
}

static void frp_server_construct(FRPServer* self, const int local_port, const SysChar* remote_host, const int remote_port) {
  self->remote_host = gethostbyname(remote_host);
  self->remote_port = remote_port;
  self->local_port = local_port;

#ifndef SYS_OS_WIN32
  signal(SIGCHLD, SIG_IGN);
#endif

  frp_create_server(self);
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
