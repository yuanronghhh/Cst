#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrIDraw.h>

static void i_draw_imp(FrIDrawInterface *iface);

SYS_DEFINE_WITH_CODE(FrContext, fr_context, SYS_TYPE_OBJECT,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_DRAW, i_draw_imp));


/* object api */
static void fr_context_construct(FrContext *self, FrSurface *surface) {
  self->surface = sys_object_ref(surface);
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

  sys_clear_pointer(&self->surface, _sys_object_unref);
  sys_clear_pointer(&(self->v.cr), fr_i_draw_destroy);

  SYS_OBJECT_CLASS(fr_context_parent_class)->dispose(o);
}

static void fr_context_class_init(FrContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_context_dispose;
}

void fr_context_init(FrContext* self) {
}

