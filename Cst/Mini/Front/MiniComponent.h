#ifndef __MINI_COMPONENT_H__
#define __MINI_COMPONENT_H__

/***
 * MiniComponent auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define MINI_TYPE_COMPONENT (mini_component_get_type())

#define MINI_COMPONENT(o) (sys_object_cast_check(o, MINI_TYPE_COMPONENT))
#define MINI_COMPONENT_CLASS(o) (sys_class_cast_check(o, MINI_TYPE_COMPONENT))
#define MINI_COMPONENT_GET_CLASS(o) sys_instance_get_class(o, MiniComponentClass)

typedef struct _MiniComponent MiniComponent;
typedef struct _MiniComponentClass MiniComponentClass;

struct _MiniComponent {
  CstComponent parent;
};

struct _MiniComponentClass {
  CstComponentClass parent;
};

SysType mini_component_get_type(void);
MiniComponent* mini_component_new(void);

SYS_END_DECLS

#endif
