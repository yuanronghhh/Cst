#include <Framework/Graph/FrImage.h>

SYS_DEFINE_TYPE(FrImage, fr_image, SYS_TYPE_OBJECT);

/* object api */
static void fr_image_construct_i(FrImage *self) {

}

FrImage* fr_image_new(void) {
  return sys_object_new(FR_TYPE_IMAGE, NULL);
}

FrImage *fr_image_new_I(void) {
  FrImage *o = fr_image_new();

  fr_image_construct_i(o);

  return o;
}

static void fr_image_dispose(SysObject* o) {
  FrImage *self = FR_IMAGE(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_image_parent_class)->dispose(o);
}

static void fr_image_class_init(FrImageClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_image_dispose;
}

void fr_image_init(FrImage* self) {
}
