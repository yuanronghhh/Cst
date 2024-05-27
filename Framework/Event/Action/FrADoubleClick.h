#ifndef __FR_ADOUBLE_CLICK_H__
#define __FR_ADOUBLE_CLICK_H__

#include <Framework/Event/Action/FrAction.h>

SYS_BEGIN_DECLS

#define FR_TYPE_ADOUBLE_CLICK (fr_adouble_click_get_type())
#define FR_ADOUBLE_CLICK(o) ((FrADoubleClick* )sys_object_cast_check(o, FR_TYPE_ADOUBLE_CLICK))
#define FR_ADOUBLE_CLICK_CLASS(o) ((FrADoubleClickClass *)sys_class_cast_check(o, FR_TYPE_ADOUBLE_CLICK))
#define FR_ADOUBLE_CLICK_GET_CLASS(o) sys_instance_get_class(o, FrADoubleClickClass)

typedef struct _FrADoubleClick FrADoubleClick;
typedef struct _FrADoubleClickClass FrADoubleClickClass;

struct _FrADoubleClickClass {
  FrActionClass parent;
};

struct _FrADoubleClick {
  FrAction parent;
};

SYS_API SysType fr_adouble_click_get_type(void);
SYS_API FrADoubleClick *fr_adouble_click_new_I(void);

SYS_END_DECLS

#endif
