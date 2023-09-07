#ifndef __SOCKET_CONNECTION__
#define __SOCKET_CONNECTION__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#define SOCKET_TYPE_CONNECTION (socket_connection_get_type())
#define SOCKET_CONNECTION(o) ((SocketConnection* )sys_object_cast_check(o, SOCKET_TYPE_CONNECTION))
#define SOCKET_CONNECTION_CLASS(o) ((SocketConnectionClass *)sys_class_cast_check(o, SOCKET_TYPE_CONNECTION))
#define SOCKET_CONNECTION_GET_CLASS(o) sys_instance_get_class(o, SocketConnectionClass)

typedef struct _SocketConnection SocketConnection;
typedef struct _SocketConnectionClass SocketConnectionClass;

struct _SocketConnection {
  SysObject parent;

  /* < private > */
  SysSocket *socket;
  struct sockaddr_in addr;
};

struct _SocketConnectionClass {
  SysObjectClass parent;
};

#define SCONN_SOCKET(conn) socket_connection_get_socket(SOCKET_CONNECTION(conn))

SYS_API SysType socket_connection_get_type(void);
SYS_API SocketConnection *socket_connection_new(void);
SYS_API SysBool socket_connection_listen(SocketConnection *self);
SYS_API SocketConnection* socket_connection_connect(const SysChar* host, const int port, SysSocket* socket);
SYS_API SocketConnection* socket_connection_new_I(const SysChar* host, const int port, SysSocket* socket);
SYS_API SocketConnection* socket_connection_accept(SocketConnection* self);
SYS_API SysSocket *socket_connection_get_socket(SocketConnection *self);

SYS_END_DECLS

#endif

