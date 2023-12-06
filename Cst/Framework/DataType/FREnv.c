#include <Framework/DataType/FREnv.h>


SYS_DEFINE_TYPE(FREnv, fr_env, SYS_TYPE_OBJECT);

SysBool fr_env_remove(FREnv *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(key != NULL, false);

  return sys_hash_table_remove(self->ht, (SysPointer)key);
}

void fr_env_set(FREnv *self, const SysChar *key, SysPointer value) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(value != NULL);
  sys_return_if_fail(key != NULL);

  sys_hash_table_insert(self->ht, (SysPointer)key, value);
}

SysPointer fr_env_get(FREnv *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);
  return sys_hash_table_lookup(self->ht, (SysPointer)key);
}

SysPointer fr_env_get_r(FREnv *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(key != NULL, NULL);

  FREnv *penv;
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

void fr_env_set_parent(FREnv *self, FREnv *parent) {
  sys_return_if_fail(self != NULL);
  if (parent) {
    sys_object_ref(parent);
  }

  if (self->parent) {
    sys_object_unref(self->parent);
  }

  self->parent = parent;

}

FREnv *fr_env_get_parent(FREnv *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->parent;
}

/* object api */
static void fr_env_construct(FREnv *self, SysHashTable *ht, FREnv *parent) {
  sys_return_if_fail(ht != NULL);

  self->ht = ht;

  fr_env_set_parent(self, parent);
}

FREnv* fr_env_new(void) {
  return sys_object_new(FR_TYPE_ENV, NULL);
}

FREnv *fr_env_new_I(SysHashTable *ht, FREnv *parent) {
  FREnv *o = fr_env_new();

  fr_env_construct(o, ht, parent);

  return o;
}

static void fr_env_dispose(SysObject* o) {
  FREnv *self = FR_ENV(o);
  sys_hash_table_unref(self->ht);
  self->ht = NULL;

  fr_env_set_parent(self, NULL);

  SYS_OBJECT_CLASS(fr_env_parent_class)->dispose(o);
}

static void fr_env_class_init(FREnvClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_env_construct;
  ocls->dispose = fr_env_dispose;
}

void fr_env_init(FREnv *self) {
}

