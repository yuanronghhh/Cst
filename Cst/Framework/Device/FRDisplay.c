#include <Framework/Event/Base/FREvent.h>
#include <Framework/Device/FRDisplayPrivate.h>


struct _FRDisplayPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRDisplay, fr_display, SYS_TYPE_OBJECT);

/* object api */
void fr_display_construct(SysObject *o) {
}

FRDisplay* fr_display_new(void) {
  return sys_object_new(FR_TYPE_DISPLAY, NULL);
}

FRDisplay *fr_display_new_I(void) {
  FRDisplay *o = fr_display_new();

  fr_display_construct(SYS_OBJECT(o));

  return o;
}

static void fr_display_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_display_parent_class)->dispose(o);
}

static void fr_display_class_init(FRDisplayClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_display_construct;
  ocls->dispose = fr_display_dispose;
}

void fr_display_init(FRDisplay* self) {
  self->priv = fr_display_get_private(self);
}
