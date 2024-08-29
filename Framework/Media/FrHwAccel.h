#ifndef __FR_HW_ACCEL_H__
#define __FR_HW_ACCEL_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_HW_ACCEL (fr_hw_accel_get_type())
#define FR_HW_ACCEL(o) ((FrHwAccel* )sys_object_cast_check(o, FR_TYPE_HW_ACCEL))
#define FR_HW_ACCEL_CLASS(o) ((FrHwAccelClass *)sys_class_cast_check(o, FR_TYPE_HW_ACCEL))
#define FR_HW_ACCEL_GET_CLASS(o) sys_instance_get_class(o, FrHwAccelClass)

struct _FrHwAccelClass {
  SysObjectClass parent;
};

struct _FrHwAccel {
  SysObject parent;

  /* <private> */
  SysInt accel_type;
  SysPointer ctx;
  SysChar *name;
};

struct _FrHwAccelContext {
  const SysChar *name;
  SysInt accel_type;
  FrMediaDecoder *decoder;

  /* <private> */

  /* AVBufferRef * */
  SysPointer ctx;
  SysInt hw_format;
};

SYS_API SysType fr_hw_accel_get_type(void);
SYS_API FrHwAccel *fr_hw_accel_new(void);

SYS_API FrHwAccel *fr_hw_accel_new_I(FrHwAccelContext *info);

SYS_END_DECLS

#endif
