#ifndef __APP_BOX_H__
#define __APP_BOX_H__

/***
 * AppBox auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define APP_TYPE_BOX (app_box_get_type())

#define APP_BOX(o) ((AppBox* )o)
#define APP_BOX_CLASS(o) ((AppBoxClass *)o)
#define APP_BOX_GET_CLASS(o) sys_instance_get_class(o, AppBoxClass)

typedef struct _AppBox AppBox;
typedef struct _AppBoxClass AppBoxClass;

struct _AppBox {
  CstComponent parent;
};

struct _AppBoxClass {
  CstComponentClass parent;
};

SYS_API SysType app_box_get_type(void);
SYS_API AppBox* app_box_new(void);

SYS_END_DECLS

#endif
