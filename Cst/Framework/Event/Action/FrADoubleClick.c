#include <Framework/Event/Action/FrADoubleClick.h>

struct _FrADoubleClickPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FrADoubleClick, fr_adouble_click, FR_TYPE_ACTION);

/* object api */
static void fr_adouble_click_construct(SysObject *o) {
  SYS_OBJECT_CLASS(fr_adouble_click_parent_class)->construct(o);

  FrADoubleClick* self = FR_ADOUBLE_CLICK(o);
  FrADoubleClickPrivate* priv = self->priv;


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
  FrADoubleClick *self = FR_ADOUBLE_CLICK(o);
  FrADoubleClickPrivate* priv = self->priv;

  SYS_OBJECT_CLASS(fr_adouble_click_parent_class)->dispose(o);
}

static void fr_adouble_click_class_init(FrADoubleClickClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_adouble_click_construct;
  ocls->dispose = fr_adouble_click_dispose;
}

void fr_adouble_click_init(FrADoubleClick *self) {
  self->priv = fr_adouble_click_get_private(self);
}

