#include <Framework/DataType/FrEnv.h>


SYS_DEFINE_TYPE(FrEnv, fr_env, SYS_TYPE_OBJECT);

SysBool fr_env_remove(FrEnv *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(key != NULL, false);

  return sys_hash_table_remove(self->ht, (SysPointer)key);
}

void fr_env_set(FrEnv *self, const SysChar *key, SysPointer value) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(value != NULL);
  sys_return_if_fail(key != NULL);

  sys_hash_table_insert(self->ht, (SysPointer)key, value);
}

SysPointer fr_env_get(FrEnv *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);
  return sys_hash_table_lookup(self->ht, (SysPointer)key);
}

SysPointer fr_env_get_r(FrEnv *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(key != NULL, NULL);

  FrEnv *penv;
  SysPointer v;

  penv = self;
  while (penv) {
    v = sys_hash_table_lookup(penv->ht, (SysPointer)key);
    if (v != NULL) {
      return v;
    }

    penv = penv->parent;
  }

  return NULL;
}

void fr_env_set_parent(FrEnv *self, FrEnv *parent) {
  sys_return_if_fail(self != NULL);
  if (parent) {
    sys_object_ref(parent);
  }

  if (self->parent) {
    sys_object_unref(self->parent);
  }

  self->parent = parent;

}

FrEnv *fr_env_get_parent(FrEnv *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->parent;
}

/* object api */
static void fr_env_construct(FrEnv *self, SysHashTable *ht, FrEnv *parent) {
  sys_return_if_fail(ht != NULL);

  self->ht = ht;

  fr_env_set_parent(self, parent);
}

FrEnv* fr_env_new(void) {
  return sys_object_new(FR_TYPE_ENV, NULL);
}

FrEnv *fr_env_new_I(SysHashTable *ht, FrEnv *parent) {
  FrEnv *o = fr_env_new();

  fr_env_construct(o, ht, parent);

  return o;
}

static void fr_env_dispose(SysObject* o) {
  FrEnv *self = FR_ENV(o);
  sys_hash_table_unref(self->ht);
  self->ht = NULL;

  fr_env_set_parent(self, NULL);

  SYS_OBJECT_CLASS(fr_env_parent_class)->dispose(o);
}

static void fr_env_class_init(FrEnvClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_env_construct;
  ocls->dispose = fr_env_dispose;
}

void fr_env_init(FrEnv *self) {
}

