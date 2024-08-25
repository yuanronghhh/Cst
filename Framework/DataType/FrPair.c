#include <Framework/DataType/FrMain.h>
#include <Framework/DataType/FrPair.h>


SYS_DEFINE_TYPE(FrPair, fr_pair, SYS_TYPE_OBJECT);

static void fr_pair_key_dispose(SysPointer key) {

}

static void fr_pair_value_dispose(SysPointer value) {
}

/* object api */
static void fr_pair_construct(FrPair *self, SysPointer key, SysPointer value) {
  self->key = key;
  self->value = value;
}

FrPair* fr_pair_new(void) {
  return sys_object_new(FR_TYPE_PAIR, NULL);
}

FrPair *fr_pair_new_I(SysPointer key, SysPointer value) {
  FrPair *o = fr_pair_new();

  fr_pair_construct(o, key, value);

  return o;
}

static void fr_pair_dispose(SysObject* o) {
  FrPair *self = FR_PAIR(o);
  FrPairClass *cls = FR_PAIR_GET_CLASS(o);

  sys_clear_pointer(&self->key, cls->key_destroy);

  if (self->value) {

    sys_clear_pointer(&self->value, cls->value_destroy);
  }
}

static void fr_pair_class_init(FrPairClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_pair_construct;
  cls->key_destroy = fr_pair_key_dispose;
  cls->value_destroy = fr_pair_value_dispose;

  ocls->dispose = fr_pair_dispose;
}

void fr_pair_init(FrPair *self) {
}
