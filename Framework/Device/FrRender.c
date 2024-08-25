#include <Framework/Device/FrRender.h>

SYS_DEFINE_TYPE(FrRender, fr_render, SYS_TYPE_OBJECT);

/* object api */
static void fr_render_construct_i(FrRender *self, FrRenderContext *info) {
}

FrRender* fr_render_new(void) {
  return sys_object_new(FR_TYPE_RENDER, NULL);
}

FrRender *fr_render_new_I(FrRenderContext *info) {
  FrRender *o = fr_render_new();

  fr_render_construct_i(o, info);

  return o;
}

static void fr_render_dispose(SysObject* o) {
  FrRender *self = FR_RENDER(o);

  UNUSED(self);


}

static void fr_render_class_init(FrRenderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_render_dispose;
}

void fr_render_init(FrRender* self) {
}
