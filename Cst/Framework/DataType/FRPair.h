#ifndef __FR_PAIR__
#define __FR_PAIR__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS


#define FR_TYPE_PAIR (fr_pair_get_type())
#define FR_PAIR(o) ((FRPair* )sys_object_cast_check(o, FR_TYPE_PAIR))
#define FR_PAIR_CLASS(o) ((FRPairClass *)sys_class_cast_check(o, FR_TYPE_PAIR))
#define FR_PAIR_GET_CLASS(o) sys_instance_get_class(o, FRPairClass)


struct _FRPair {
  SysObject parent;

  SysPointer key;
  SysPointer value;
  /* <private> */
};

struct _FRPairClass {
  SysObjectClass parent;
  void (*construct) (FRPair *o, SysPointer key, SysPointer value);

  /* virtual */
  void (*key_destroy) (SysPointer key);
  void (*value_destroy) (SysPointer value);
};


SYS_API SysType fr_pair_get_type(void);
SYS_API FRPair * fr_pair_new(void);
SYS_API FRPair *fr_pair_new_I(SysPointer key, SysPointer value);

SYS_END_DECLS

#endif

