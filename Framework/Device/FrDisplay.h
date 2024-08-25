#ifndef __FR_DISPLAY_H__
#define __FR_DISPLAY_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS


#define FR_TYPE_DISPLAY (fr_display_get_type())
#define FR_DISPLAY(o) ((FrDisplay* )sys_object_cast_check(o, FR_TYPE_DISPLAY))
#define FR_DISPLAY_CLASS(o) ((FrDisplayClass *)sys_class_cast_check(o, FR_TYPE_DISPLAY))
#define FR_DISPLAY_GET_CLASS(o) sys_instance_get_class(o, FrDisplayClass)

struct _FrDisplayClass {
  SysObjectClass parent;
};

struct _FrDisplay {
  SysObject parent;

  /* SDL_DisplayID */
  SysPointer ctx;

  /* Display */
  SysPointer native_ctx;
};

SYS_API SysType fr_display_get_type(void);
SYS_API FrDisplay* fr_display_new_I (void);

#define fr_display_get_native_display(o) fr_i_window_get_native_display(o)

SYS_END_DECLS

#endif

