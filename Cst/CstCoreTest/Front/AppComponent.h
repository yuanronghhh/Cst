#ifndef __APP_COMPONENT_H__
#define __APP_COMPONENT_H__

/***
 * AppComponent auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define APP_TYPE_COMPONENT (app_component_get_type())

#define APP_COMPONENT(o) ((AppComponent* )sys_object_cast_check(o, APP_TYPE_COMPONENT))
#define APP_COMPONENT_CLASS(o) ((AppComponentClass *)sys_object_cast_check(o, APP_TYPE_COMPONENT))
#define APP_COMPONENT_GET_CLASS(o) sys_instance_get_class(o, AppComponentClass)

typedef struct _AppComponent AppComponent;
typedef struct _AppComponentClass AppComponentClass;
typedef struct _AppComponentPrivate AppComponentPrivate;

struct _AppComponent {
  CstComponent parent;
  AppComponentPrivate *priv;
};

struct _AppComponentClass {
  CstComponentClass parent;
};

SYS_API SysType app_component_get_type(void);
SYS_API AppComponent* app_component_new(void);

SYS_END_DECLS

#endif
