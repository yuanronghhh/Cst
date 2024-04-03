#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrDraw.h>
#include <Framework/Graph/FrIDraw.h>

SYS_DEFINE_TYPE(FrContext, fr_context, SYS_TYPE_OBJECT);

void fr_context_fill_bound(FrContext* self, const FrBound *bound) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(bound != NULL);

  FrDrawBrush* cr = self->v.cr;
  FrIDrawInterface* idraw_iface = fr_draw_get_iface();

  idraw_iface->rectangle(cr, bound->x, bound->y, bound->width, bound->height);
  idraw_iface->fill(cr);
}

void fr_context_fill_background (FrContext *self, SysInt width, SysInt height) {
  FrDrawBrush *cr = self->v.cr;
  FrIDrawInterface *idraw_iface = fr_draw_get_iface();

  idraw_iface->set_source_rgba(cr, 1.0, 1.0, 1.0, 0.8);
  idraw_iface->rectangle(cr, 0, 0, width, height);
  idraw_iface->paint(cr);
}

void fr_context_stoke_debug (FrContext *self, SysInt i) {
  FrDrawBrush *cr = self->v.cr;
  FrIDrawInterface *idraw_iface = fr_draw_get_iface();

  idraw_iface->set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);
  idraw_iface->rectangle(cr, 20 + i * 5, 30 + i * 5, 200, 100);
  idraw_iface->stroke(cr);
}

void fr_context_set_color(FrContext * self, FrColor *color) {

  fr_i_draw_set_source_rgba(self->v.cr, color->r, color->g, color->b, color->a);
}

void fr_context_set_source_surface (FrContext* self, FrSurface* surface, SysDouble x, SysDouble y) {
  FrDrawSurface *draw_surface = fr_surface_get_draw_surface(surface);

  fr_i_draw_set_source_surface(self->v.cr, draw_surface, x, y);
}

void fr_context_rectangle (FrContext* self,SysDouble x,SysDouble y,SysDouble width,SysDouble height) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_rectangle(self->v.cr, x, y, width, height);
}

void fr_context_stroke_mp(FrContext* self, const FrBound *bound, const FrSInt4* m4, const FrSInt4* p4) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(m4 != NULL);
  sys_return_if_fail(p4 != NULL);
  sys_return_if_fail(bound != NULL);

  FrDrawBrush* cr = self->v.cr;
  FrIDrawInterface* idraw_iface = fr_draw_get_iface();

  SysInt x = bound->x + m4->m3;
  SysInt y = bound->y + m4->m0;
  SysInt width = bound->width + p4->m1 + p4->m3;
  SysInt height = bound->height + p4->m0 + p4->m2;

  idraw_iface->rectangle(cr, x, y, width, height);
  idraw_iface->stroke(cr);
}

void fr_context_stroke(FrContext *self) {
  FrIDrawInterface* idraw_iface = fr_draw_get_iface();

  idraw_iface->stroke(self->v.cr);
}

void fr_context_clip (FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_clip (self->v.cr);
}

void fr_context_paint (FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_paint (self->v.cr);
}

void fr_context_move_to (FrContext* self,SysDouble x,SysDouble y) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_move_to(self->v.cr, x, y);
}

void fr_context_layout_layout(FrContext* self, PangoLayout* layout) {

  pango_cairo_update_layout(self->v.cr, layout);
}

void fr_context_save(FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_save(self->v.cr);
}

void fr_context_restore(FrContext* self) {
  sys_return_if_fail(self != NULL);

  fr_i_draw_restore(self->v.cr);
}

/* font */
void fr_context_draw_text(FrContext* self, PangoLayout* layout, SysInt x, SysInt y) {
  FrIDrawInterface* iface = fr_draw_get_iface();

  iface->move_to(self->v.cr, x, y);
  iface->show_layout(self->v.cr, layout);
}

void fr_context_show_text(FrContext* self, PangoLayout *layout,
  SysInt x, SysInt y, SysInt m1, SysInt m0) {
  FrIDrawInterface* iface = fr_draw_get_iface();

  iface->move_to(self->v.cr, x + m1, y + m0);
  iface->show_layout(self->v.cr, layout);
}

void fr_context_update_layout(FrContext* self, PangoLayout* layout) {

  fr_i_draw_update_layout(self->v.cr, layout);
}

void fr_context_overlay(FrContext *self, FrSurface *surface, SysInt x, SysInt y) {
  FrDrawSurface *draw_surface = fr_surface_get_draw_surface(surface);

  fr_i_draw_context_overlay(self->v.cr, draw_surface, 0, 0);
}

/* object api */
static void fr_context_construct(FrContext *self, FrSurface *surface) {
  FrDrawSurface *draw_surface = fr_surface_get_draw_surface(surface);

  self->v.cr = fr_i_draw_create(draw_surface);
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

  sys_clear_pointer(&self->v.cr, fr_i_draw_destroy);

  SYS_OBJECT_CLASS(fr_context_parent_class)->dispose(o);
}

static void fr_context_class_init(FrContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_context_dispose;
}

void fr_context_init(FrContext* self) {
}

