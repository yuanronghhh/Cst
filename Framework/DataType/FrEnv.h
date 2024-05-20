#ifndef __FR_ENV_H__
#define __FR_ENV_H__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_ENV (fr_env_get_type())
#define FR_ENV(o) ((FrEnv* )sys_object_cast_check(o, FR_TYPE_ENV))
#define FR_ENV_CLASS(o) ((FrEnvClass *)sys_class_cast_check(o, FR_TYPE_ENV))
#define FR_ENV_GET_CLASS(o) sys_instance_get_class(o, FrEnvClass)

struct _FrEnvClass {
  SysObjectClass parent;

  void (*construct)(FrEnv *o, SysHashTable *ht, FrEnv *parent);
};

struct _FrEnv {
  SysObject unowned;

  /* <private> */
  FrEnv *parent;
  SysHashTable *ht;
};

SYS_API SysType fr_env_get_type(void);
SYS_API FrEnv *fr_env_new_I(SysHashTable *ht, FrEnv *parent);

SYS_API SysBool fr_env_remove(FrEnv *self, const SysChar *key);
SYS_API void fr_env_set(FrEnv *env, const SysChar *key, SysPointer value);
SYS_API SysPointer fr_env_get(FrEnv *env, const SysChar *key);
SYS_API SysPointer fr_env_get_r(FrEnv *env, const SysChar *key);

SYS_API void fr_env_set_parent(FrEnv *env, FrEnv *parent);
SYS_API FrEnv *fr_env_get_parent(FrEnv *env);

SYS_END_DECLS

#endif

