#include <Framework/DataType/FRMain.h>
#include <Framework/DataType/FRPair.h>


SYS_DEFINE_TYPE(FRPair, fr_pair, SYS_TYPE_OBJECT);

static void fr_pair_key_destroy(SysPointer key) {

}

static void fr_pair_value_destroy(SysPointer value) {

}

/* object api */
static void fr_pair_construct(FRPair *self, SysPointer key, SysPointer value) {
  self->key = key;
  self->value = value;
}

FRPair* fr_pair_new(void) {
  return sys_object_new(FR_TYPE_PAIR, NULL);
}

FRPair *fr_pair_new_I(SysPointer key, SysPointer value) {
  FRPair *o = fr_pair_new();

  fr_pair_construct(o, key, value);

  return o;
}

static void fr_pair_dispose(SysObject* o) {
  FRPair *self = FR_PAIR(o);
  FRPairClass *cls = FR_PAIR_GET_CLASS(o);

  sys_clear_pointer(&self->key, cls->key_destroy);

  if (self->value) {

    sys_clear_pointer(&self->value, cls->value_destroy);
  }

  SYS_OBJECT_CLASS(fr_pair_parent_class)->dispose(o);
}

static void fr_pair_class_init(FRPairClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_pair_construct;
  cls->key_destroy = fr_pair_key_destroy;
  cls->value_destroy = fr_pair_value_destroy;

  ocls->dispose = fr_pair_dispose;
}

void fr_pair_init(FRPair *self) {
}
