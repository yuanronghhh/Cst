#include <Framework/DataType/FRMain.h>
#include <Framework/DataType/FRPair.h>


SYS_DEFINE_TYPE(FRPair, fr_pair, SYS_TYPE_OBJECT);

/* object api */
static void fr_pair_construct(FRPair *self, const SysChar *key, SysPointer value) {
  self->key = sys_strdup(key);
  self->value = value;
}

FRPair* fr_pair_new(void) {
  return sys_object_new(FR_TYPE_PAIR, NULL);
}

FRPair *fr_pair_new_I(const SysChar *key, SysPointer value) {
  FRPair *o = fr_pair_new();

  fr_pair_construct(o, key, value);

  return o;
}

static void fr_pair_dispose(SysObject* o) {
  FRPair *self = FR_PAIR(o);

  sys_clear_pointer(&self->key, sys_free);

  SYS_OBJECT_CLASS(fr_pair_parent_class)->dispose(o);
}

static void fr_pair_class_init(FRPairClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_pair_construct;
  ocls->dispose = fr_pair_dispose;
}

void fr_pair_init(FRPair *self) {
}
