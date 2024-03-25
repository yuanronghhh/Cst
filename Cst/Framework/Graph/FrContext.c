#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Graph/FrDraw.h>

SYS_DEFINE_TYPE(FrContext, fr_context, SYS_TYPE_OBJECT);

void fr_context_fill_background (FrContext *self, SysInt width, SysInt height) {
  FrDrawContext *cr = self->cr;

  fr_i_draw_set_sourcce_rgba(cr, 1.0, 1.0, 1.0, 0.5);
  fr_i_draw_rectangle(cr, 0, 0, width, height);
  fr_i_draw_paint(cr);
}

/* object api */
static void fr_context_construct(FrContext *self, FrSurface *surface) {
  self->cr = fr_surface_create_draw_cr(surface);
}

FrContext* fr_context_new(void) {
  return sys_object_new(FR_TYPE_CONTEXT, NULL);
}

FrContext *fr_context_new_I(FrSurface *surface) {
  FrContext *o = fr_context_new();

  fr_context_construct(o, surface);

  return o;
}

static void fr_context_dispose(SysObject* o) {
  FrContext *self = FR_CONTEXT(o);

  sys_clear_pointer(&self->cr, fr_draw_context_destroy);

  SYS_OBJECT_CLASS(fr_context_parent_class)->dispose(o);
}

static void fr_context_class_init(FrContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_context_dispose;
}

void fr_context_init(FrContext* self) {
}

