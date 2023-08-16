#ifndef __FR_ENV_H__
#define __FR_ENV_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_ENV (fr_env_get_type())
#define FR_ENV(o) ((FREnv* )sys_object_cast_check(o, FR_TYPE_ENV))
#define FR_ENV_CLASS(o) ((FREnvClass *)sys_class_cast_check(o, FR_TYPE_ENV))
#define FR_ENV_GET_CLASS(o) sys_instance_get_class(o, FREnvClass)

struct _FREnvClass {
  SysObjectClass parent;

  void (*construct)(FREnv *o, SysHashTable *ht, FREnv *parent);
};

struct _FREnv {
  SysObject parent;
  FREnvPrivate *priv;
};

SYS_API SysType fr_env_get_type(void);
SYS_API FREnv *fr_env_new_I(SysHashTable *ht, FREnv *parent);

SYS_API SysBool fr_env_remove(FREnv *self, const SysChar *key);
SYS_API void fr_env_set(FREnv *env, const SysChar *key, SysPointer value);
SYS_API SysPointer fr_env_get(FREnv *env, const SysChar *key);
SYS_API SysPointer fr_env_get_r(FREnv *env, const SysChar *key);

SYS_API void fr_env_set_parent(FREnv *env, FREnv *parent);
SYS_API FREnv *fr_env_get_parent(FREnv *env);

SYS_END_DECLS

#endif

