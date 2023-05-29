#ifndef __APP_BUTTON_H__
#define __APP_BUTTON_H__

/***
 * AppButton auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define APP_TYPE_BUTTON (app_button_get_type())

#define APP_BUTTON(o) ((AppButton* )o)
#define APP_BUTTON_CLASS(o) ((AppButtonClass *)o)
#define APP_BUTTON_GET_CLASS(o) sys_instance_get_class(o, AppButtonClass)

typedef struct _AppButton AppButton;
typedef struct _AppButtonClass AppButtonClass;
typedef struct _AppButtonPrivate AppButtonPrivate;


struct _AppButton {
  CstComponent parent;
  AppButtonPrivate *priv;
};

struct _AppButtonClass {
  CstComponentClass parent;
};

SYS_API SysType app_button_get_type(void);
SYS_API AppButton* app_button_new(void);

SYS_END_DECLS

#endif
