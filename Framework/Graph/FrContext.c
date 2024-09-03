#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Graph/FrContext.h>

SYS_DEFINE_TYPE(FrContext, fr_context, SYS_TYPE_OBJECT);

 void fr_context_stroke_mp(FrContext* self, const FrBound *bound, const FrSInt4* m4, const FrSInt4* p4) {
   sys_return_if_fail(self != NULL);
   sys_return_if_fail(m4 != NULL);
   sys_return_if_fail(p4 != NULL);
   sys_return_if_fail(bound != NULL);
 
   FrIDrawInterface* idraw_iface = fr_i_draw_get_g_iface();

   SysInt x = bound->x + m4->m3;
   SysInt y = bound->y + m4->m0;
   SysInt width = bound->width + p4->m1 + p4->m3;
   SysInt height = bound->height + p4->m0 + p4->m2;

   idraw_iface->rectangle(self, x, y, width, height);
   idraw_iface->stroke(self);
 }

 void fr_context_rectangle_red(FrContext* cr, SysInt x, SysInt y) {
   FrIDrawInterface* idraw_iface = fr_i_draw_get_g_iface();
   FrColor color = { 1.0, 0.0, 0.0, 1.0 };

   idraw_iface->set_color(cr, &color);
   idraw_iface->rectangle(cr, x, y, 200, 100);
   idraw_iface->stroke(cr);
 }

 void fr_context_stoke_debug(FrContext* cr, SysInt i) {
   FrIDrawInterface* idraw_iface = fr_i_draw_get_g_iface();
   FrColor color = { 1.0, 0.0, 0.0, 1.0 };

   idraw_iface->set_color(cr, &color);
   idraw_iface->rectangle(cr, 20 + i * 5, 30 + i * 5, 200, 100);
   idraw_iface->stroke(cr);
 }

 void fr_context_fill_bound(FrContext* self, const FrBound* bound) {
   sys_return_if_fail(self != NULL);
   sys_return_if_fail(bound != NULL);
   FrIDrawInterface* idraw_iface = fr_i_draw_get_g_iface();

   idraw_iface->rectangle(self, bound->x, bound->y, bound->width, bound->height);
   idraw_iface->fill(self);
 }

void fr_context_show_layout_m(FrContext* self,
    PangoLayout *layout,
    SysInt x,
    SysInt y, 
    SysInt m1, 
    SysInt m0) {
  FrIDrawInterface* idraw_iface = fr_i_draw_get_g_iface();

  idraw_iface->move_to(self, x + m1, y + m0);
  idraw_iface->show_layout(self, layout);
}

void fr_context_show_text_p (FrContext* self,
    SysInt x, 
    SysInt y, 
    const SysChar *text) {

  FrIDrawInterface* idraw_iface = fr_i_draw_get_g_iface();

  idraw_iface->move_to(self, x , y);
  idraw_iface->show_text(self, text);
}

/* object api */
static void fr_context_construct(FrContext *self, FrSurface *surface) {
  self->surface = sys_object_ref(surface);

  fr_i_draw_create(self, surface);
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
  fr_i_draw_destroy(self);

  SYS_OBJECT_CLASS(fr_context_parent_class)->dispose(o);
}

static void fr_context_class_init(FrContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_context_dispose;
}

void fr_context_init(FrContext* self) {
}

