#ifndef __FR_DISPLAY_H__
#define __FR_DISPLAY_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS


#define FR_TYPE_DISPLAY (fr_display_get_type())
#define FR_DISPLAY(o) ((FRDisplay* )sys_object_cast_check(o, FR_TYPE_DISPLAY))
#define FR_DISPLAY_CLASS(o) ((FRDisplayClass *)sys_class_cast_check(o, FR_TYPE_DISPLAY))
#define FR_DISPLAY_GET_CLASS(o) sys_instance_get_class(o, FRDisplayClass)


struct _FRDisplayClass {
  SysObjectClass parent;
};

struct _FRDisplay {
  SysObject parent;
};

SYS_API SysType fr_display_get_type(void);
SYS_API FRDisplay* fr_display_new_I (void);

#if SYS_OS_WIN32

#elif SYS_OS_UNIX

SYS_API Display* fr_display_get_x11_display(FRDisplay *display);

#endif

SYS_END_DECLS

#endif

