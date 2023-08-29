#include <Framework/Device/FRDisplay.h>
#include <Framework/Event/Base/FREvent.h>


SYS_DEFINE_TYPE(FRDisplay, fr_display, SYS_TYPE_OBJECT);

/* object api */
FRDisplay* fr_display_new(void) {
  return sys_object_new(FR_TYPE_DISPLAY, NULL);
}

FRDisplay *fr_display_new_I(void) {
  FRDisplay *o = fr_display_new();

  return o;
}

static void fr_display_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_display_parent_class)->dispose(o);
}

static void fr_display_class_init(FRDisplayClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_display_dispose;
}

void fr_display_init(FRDisplay* self) {
}
