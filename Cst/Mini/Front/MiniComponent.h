#ifndef __MINI_COMPONENT_H__
#define __MINI_COMPONENT_H__

/***
 * MiniComponent auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define MINI_TYPE_COMPONENT (mini_component_get_type())

#define MINI_COMPONENT(o) ((MiniComponent* )o)
#define MINI_COMPONENT_CLASS(o) ((MiniComponentClass *)o)
#define MINI_COMPONENT_GET_CLASS(o) sys_instance_get_class(o, MiniComponentClass)

typedef struct _MiniComponent MiniComponent;
typedef struct _MiniComponentClass MiniComponentClass;
typedef struct _MiniComponentPrivate MiniComponentPrivate;

struct _MiniComponent {
  CstComponent parent;
  MiniComponentPrivate *priv;
};

struct _MiniComponentClass {
  CstComponentClass parent;
};

SysType mini_component_get_type(void);
MiniComponent* mini_component_new(void);

SYS_END_DECLS

#endif
