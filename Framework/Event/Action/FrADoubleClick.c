#include <Framework/Event/Action/FrADoubleClick.h>

SYS_DEFINE_TYPE(FrADoubleClick, fr_adouble_click, FR_TYPE_ACTION);

/* object api */
static void fr_adouble_click_construct(SysObject *o) {
}

FrADoubleClick* fr_adouble_click_new(void) {
  return sys_object_new(FR_TYPE_ADOUBLE_CLICK, NULL);
}

FrADoubleClick *fr_adouble_click_new_I(void) {
  FrADoubleClick *o = fr_adouble_click_new();

  fr_adouble_click_construct(SYS_OBJECT(o));

  return o;
}

static void fr_adouble_click_dispose(SysObject* o) {

}

static void fr_adouble_click_class_init(FrADoubleClickClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_adouble_click_dispose;
}

void fr_adouble_click_init(FrADoubleClick *self) {
}

