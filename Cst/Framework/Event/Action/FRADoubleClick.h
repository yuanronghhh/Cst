#ifndef __FR_ADOUBLE_CLICK_H__
#define __FR_ADOUBLE_CLICK_H__

#include <Framework/Event/Action/FRAction.h>

SYS_BEGIN_DECLS

#define FR_TYPE_ADOUBLE_CLICK (fr_adouble_click_get_type())
#define FR_ADOUBLE_CLICK(o) ((FRADoubleClick* )sys_object_cast_check(o, FR_TYPE_ADOUBLE_CLICK))
#define FR_ADOUBLE_CLICK_CLASS(o) ((FRADoubleClickClass *)sys_class_cast_check(o, FR_TYPE_ADOUBLE_CLICK))
#define FR_ADOUBLE_CLICK_GET_CLASS(o) sys_instance_get_class(o, FRADoubleClickClass)

typedef struct _FRADoubleClick FRADoubleClick;
typedef struct _FRADoubleClickClass FRADoubleClickClass;
typedef struct _FRADoubleClickPrivate FRADoubleClickPrivate;

struct _FRADoubleClickClass {
  FRActionClass parent;
};

struct _FRADoubleClick {
  FRAction parent;

  FRADoubleClickPrivate *priv;
};

SYS_API SysType fr_adouble_click_get_type(void);
SYS_API FRADoubleClick *fr_adouble_click_new_I(void);

SYS_END_DECLS

#endif
