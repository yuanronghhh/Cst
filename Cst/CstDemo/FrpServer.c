#include <CstDemo/FrpServer.h>

#if SYS_OS_UNIX
#define SOCKET SysUIntPtr
#endif

#define BUFF_SIZE 8192

SYS_DEFINE_TYPE(FRPServer, frp_server, SYS_TYPE_OBJECT);

static void close_socket(SOCKET s) {
#if SYS_OS_WIN32
  closesocket(s);
#else
  close(s);
#endif
}

static SysBool frp_create_server(FRPServer *self) {
  int r;
  int optval = 1;

  memset(&self->server_addr, 0, sizeof(self->server_addr));

  self->server_addr.sin_port = htons(self->local_port);
  self->server_addr.sin_family = AF_INET;
  self->server_addr.sin_addr.s_addr = INADDR_ANY;

  self->server_socket = sys_socket(AF_INET, SOCK_STREAM, 0);
  if (self->server_socket < 0) {

    sys_error_N("socket: %d", self->server_socket);
    return false;
  }

#if SYS_OS_WIN32
  r = setsockopt(self->server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
#else
  r = setsockopt(self->server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif
  if (r < 0) {

    sys_error_N("setsockopt: %d", r);
    return false;
  }

  r = bind(self->server_socket, (struct sockaddr*)&self->server_addr, sizeof(self->server_addr));
  if (r < 0) {

    sys_error_N("bind: %d", r);
    return false;
  }

  r = listen(self->server_socket, 1);
  if (r < 0) {
    sys_error_N("listen: %d", r);
    return false;
  }

  return true;
}

static SysBool frp_accept_clients(FRPServer* self) {
  int client_addr_size = sizeof(struct sockaddr_in);

  self->client_socket = accept(self->server_socket, (struct sockaddr*)&self->client_addr, &client_addr_size);
  if (self->client_socket < 0) {
    if (errno != EINTR) {

      sys_warning_N("accept: %d", self->client_socket);
    }
  }

  return true;
}

static SysBool frp_build_tunnel(FRPServer *self) {
  int r;
  if (self->remote_host == NULL) {
    sys_error_N("%s", "remote_host is NULL");
    return false;
  }

  memset(&self->remote_addr, 0, sizeof(self->remote_addr));

  self->remote_addr.sin_family = AF_INET;
  self->remote_addr.sin_port = htons(self->remote_port);

  memcpy(&self->remote_addr.sin_addr.s_addr, self->remote_host->h_addr, self->remote_host->h_length);

  self->remote_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (self->remote_socket < 0) {

    sys_error_N("socket: %d", self->remote_socket);
    return false;
  }

  r = connect(self->remote_socket, (struct sockaddr*)&self->remote_addr, sizeof(self->remote_addr));
  if (r < 0) {

    sys_error_N("connect: %d", r);
    return false;
  }

  return true;
}

static int frp_get_fd(FRPServer* self) {
  SOCKET fd = max(self->client_socket, self->remote_socket);
  return (int)(fd + 1);
}

static SysBool frp_handle_tunnel(FRPServer* self) {
  fd_set io;
  struct timeval tv;
  int r;
  SysChar buffer[BUFF_SIZE] = { 0 };

  tv.tv_sec = 10;
  tv.tv_usec = 0;

  for (;;) {
    FD_ZERO(&io);

    FD_SET(self->remote_socket, &io);
    FD_SET(self->client_socket, &io);

    int nfds = frp_get_fd(self);
    r = select(nfds, &io, NULL, NULL, &tv);

    if (r < 0) {

      sys_error_N("select: %d", r);
      break;
    }

    if (FD_ISSET(self->client_socket, &io)) {
      r = recv(self->client_socket, buffer, sizeof(buffer), 0);
      if (r < 0) {
        sys_warning_N("recv(client_socket): %d", r);

        close_socket(self->client_socket);
        close_socket(self->remote_socket);
        return false;
      }

      if (r == 0) {

        close_socket(self->client_socket);
        close_socket(self->remote_socket);
        return false;
      }

      send(self->remote_socket, buffer, r, 0);
    }

    if (FD_ISSET(self->remote_socket, &io)) {
      r = recv(self->remote_socket, buffer, sizeof(buffer), 0);
      if (r < 0) {
        sys_warning_N("recv(remote_socket): %d", r);

        close_socket(self->client_socket);
        close_socket(self->remote_socket);
        return false;
      }

      if (r == 0) {
        close_socket(self->client_socket);
        close_socket(self->remote_socket);

        return true;
      }

      send(self->client_socket, buffer, r, 0);
    }
  }

  return true;
}

static SysBool frp_handle_clients(FRPServer *self) {
  if (!frp_build_tunnel(self)) {
    return false;
  }

  return frp_handle_tunnel(self);
}

void frp_server_run(FRPServer *s) {
  while(frp_accept_clients(s)) {

    frp_handle_clients(s);
  }

  close_socket(s->server_socket);
}

/* object api */
static void frp_server_dispose(SysObject* o) {
  FRPServer *self = FRP_SERVER(o);

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
