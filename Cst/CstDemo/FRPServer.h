#ifndef __FRP_SERVER__
#define __FRP_SERVER__

#include <CstDemo/SocketConnection.h>

SYS_BEGIN_DECLS

#define FRP_TYPE_SERVER (frp_server_get_type())
#define FRP_SERVER(o) ((FRPServer* )sys_object_cast_check(o, FRP_TYPE_SERVER))
#define FRP_SERVER_CLASS(o) ((FRPServerClass *)sys_class_cast_check(o, FRP_TYPE_SERVER))
#define FRP_SERVER_GET_CLASS(o) sys_instance_get_class(o, FRPServerClass)

typedef struct _FRPServer FRPServer;
typedef struct _FRPServerClass FRPServerClass;

struct _FRPServer {
  SysObject parent;

  /* < private > */
  SocketConnection* server_conn;

#if USE_OPENSSL
  SSL_CTX* server_ctx;
  SSL_CTX *client_ctx;
#endif

  /* pair */
  SocketConnection *rconn;
  SocketConnection *cconn;

  SysInt maxfd;
  fd_set rfds;

  /* options */
  SysChar *remote_host;
  int remote_port;
  int local_port;
};

struct _FRPServerClass {
  SysObjectClass parent;
};

SYS_API SysType frp_server_get_type(void);
SYS_API FRPServer *frp_server_new(void);
SYS_API FRPServer *frp_server_new_I(const int local_port, const SysChar* remote_host, const int remote_port);
SYS_API void frp_server_run(FRPServer *s);

SYS_END_DECLS

#endif

