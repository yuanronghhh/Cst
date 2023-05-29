#ifndef __LAYOUT_COMPONENT_H__
#define __LAYOUT_COMPONENT_H__

/***
 * LayoutComponent auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define LAYOUT_TYPE_COMPONENT (layout_component_get_type())

#define LAYOUT_COMPONENT(o) ((LayoutComponent* )o)
#define LAYOUT_COMPONENT_CLASS(o) ((LayoutComponentClass *)o)
#define LAYOUT_COMPONENT_GET_CLASS(o) sys_instance_get_class(o, LayoutComponentClass)

typedef struct _LayoutComponent LayoutComponent;
typedef struct _LayoutComponentClass LayoutComponentClass;
typedef struct _LayoutComponentPrivate LayoutComponentPrivate;


struct _LayoutComponent {
  CstComponent parent;
  LayoutComponentPrivate *priv;
};

struct _LayoutComponentClass {
  CstComponentClass parent;
};

SYS_API SysType layout_component_get_type(void);
SYS_API LayoutComponent* layout_component_new(void);

SYS_END_DECLS

#endif
