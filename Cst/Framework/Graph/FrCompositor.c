#include <Framework/Graph/FrCompositor.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrSurface.h>

SYS_DEFINE_TYPE(FrCompositor, fr_compositor, SYS_TYPE_OBJECT);

void fr_compositor_surface_overlay(
  FrSurface* pn,
  FrContext* pb_cr) {

  fr_context_overlay(pb_cr, pn, 0, 0);
  fr_context_paint(pb_cr);
}

/* object api */
static void fr_compositor_construct(FrCompositor *self) {
}

FrCompositor* fr_compositor_new(void) {
  return sys_object_new(FR_TYPE_COMPOSITOR, NULL);
}

FrCompositor *fr_compositor_new_I(void) {
  FrCompositor *o = fr_compositor_new();

  fr_compositor_construct(o);

  return o;
}

static void fr_compositor_dispose(SysObject* o) {
  // FrCompositor *self = FR_COMPOSITOR(o);

  SYS_OBJECT_CLASS(fr_compositor_parent_class)->dispose(o);
}

static void fr_compositor_class_init(FrCompositorClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_compositor_dispose;
}

void fr_compositor_init(FrCompositor* self) {
}
