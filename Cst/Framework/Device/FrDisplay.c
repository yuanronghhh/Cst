#include <Framework/Device/FrDisplay.h>
#include <Framework/Event/Base/FrEvent.h>


SYS_DEFINE_TYPE(FrDisplay, fr_display, SYS_TYPE_OBJECT);


/* object api */
FrDisplay* fr_display_new(void) {
  return sys_object_new(FR_TYPE_DISPLAY, NULL);
}

FrDisplay *fr_display_new_I(void) {
  FrDisplay *o = fr_display_new();

  return o;
}

static void fr_display_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_display_parent_class)->dispose(o);
}

static void fr_display_class_init(FrDisplayClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_display_dispose;
}

void fr_display_init(FrDisplay* self) {
}
