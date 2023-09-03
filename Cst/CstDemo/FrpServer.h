#ifndef __FRP_SERVER__
#define __FRP_SERVER__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#define FRP_TYPE_SERVER (frp_server_get_type())
#define FRP_SERVER(o) ((FRPServer* )sys_object_cast_check(o, FRP_TYPE_SERVER))
#define FRP_SERVER_CLASS(o) ((FRPServerClass *)sys_class_cast_check(o, FRP_TYPE_SERVER))
#define FRP_SERVER_GET_CLASS(o) sys_instance_get_class(o, FRPServerClass)

typedef struct _FRPServer FRPServer;
typedef struct _FRPServerClass FRPServerClass;

struct _FRPServer {
  SysObject parent;

  SysSocket* server_socket;
  SysSocket* client_socket;
  SysSocket* remote_socket;

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  struct sockaddr_in remote_addr;
  struct hostent* remote_host;

  SysUShort local_port;
  SysUShort remote_port;
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

