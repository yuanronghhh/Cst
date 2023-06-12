#include <Framework/Event/Action/FRADoubleClick.h>

struct _FRADoubleClickPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRADoubleClick, fr_adouble_click, FR_TYPE_ACTION);

/* object api */
static void fr_adouble_click_construct(SysObject *o) {
  SYS_OBJECT_CLASS(fr_adouble_click_parent_class)->construct(o);

  FRADoubleClick* self = FR_ADOUBLE_CLICK(o);
  FRADoubleClickPrivate* priv = self->priv;


}

FRADoubleClick* fr_adouble_click_new(void) {
  return sys_object_new(FR_TYPE_ADOUBLE_CLICK, NULL);
}

FRADoubleClick *fr_adouble_click_new_I(void) {
  FRADoubleClick *o = fr_adouble_click_new();

  fr_adouble_click_construct(SYS_OBJECT(o));

  return o;
}

static void fr_adouble_click_dispose(SysObject* o) {
  FRADoubleClick *self = FR_ADOUBLE_CLICK(o);
  FRADoubleClickPrivate* priv = self->priv;

  SYS_OBJECT_CLASS(fr_adouble_click_parent_class)->dispose(o);
}

static void fr_adouble_click_class_init(FRADoubleClickClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_adouble_click_construct;
  ocls->dispose = fr_adouble_click_dispose;
}

void fr_adouble_click_init(FRADoubleClick *self) {
  self->priv = fr_adouble_click_get_private(self);
}

