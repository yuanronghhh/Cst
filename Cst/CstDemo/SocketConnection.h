#ifndef __SOCKET_CONNECTION__
#define __SOCKET_CONNECTION__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#define SOCKET_TYPE_CONNECTION (socket_connection_get_type())
#define SOCKET_CONNECTION(o) ((SocketConnection* )sys_object_cast_check(o, SOCKET_TYPE_CONNECTION))
#define SOCKET_CONNECTION_CLASS(o) ((SocketConnectionClass *)sys_class_cast_check(o, SOCKET_TYPE_CONNECTION))
#define SOCKET_CONNECTION_GET_CLASS(o) sys_instance_get_class(o, SocketConnectionClass)
#define SCONN_SOCKET_ID(conn) sys_socket_get_fd(socket_connection_get_socket(SOCKET_CONNECTION(conn)))

typedef struct _SocketConnection SocketConnection;
typedef struct _SocketConnectionClass SocketConnectionClass;
typedef SysSSize (*SocketConnectionFunc)(SocketConnection *self, SysPointer user_data);

struct _SocketConnection {
  SysObject parent;

  /* < private > */
  SysSocket *socket;
  struct sockaddr_in addr;
  SocketConnectionFunc func;
};

struct _SocketConnectionClass {
  SysObjectClass parent;

};

SYS_API SysType socket_connection_get_type(void);
SYS_API SocketConnection *socket_connection_new(void);
SYS_API SysBool socket_connection_listen(SocketConnection *self);
SYS_API SocketConnection *socket_connection_connect(const SysChar* host, const int port, SysSocket *socket, SocketConnectionFunc func);
SYS_API SocketConnection* socket_connection_new_I(const SysChar* host, const int port, SysSocket *socket, SocketConnectionFunc func);
SYS_API SocketConnection* socket_connection_accept(SocketConnection* self, SocketConnectionFunc func);
SYS_API SysSocket *socket_connection_get_socket(SocketConnection *self);
SYS_API SysSize socket_connection_handle(SocketConnection *self, SysPointer user_data);

SYS_END_DECLS

#endif

