#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrCairoDraw.h>


SYS_DEFINE_INTERFACE(FrIDraw, fr_i_draw, SYS_TYPE_OBJECT);

/* object api */
void fr_i_draw_default_init(FrIDrawInterface* iface) {
}

FrDrawContext* fr_i_draw_create_cr_default (FrDraw *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return FR_I_DRAW_GET_IFACE(self)->create_cr_default(self);
}

void fr_i_draw_set_color (FrDraw *self, FrDrawContext *cr, SysDouble r, SysDouble  g, SysDouble b, SysDouble a) {
  sys_return_if_fail(self != NULL);

  FR_I_DRAW_GET_IFACE(self)->set_source_rgba(self, cr, r, g, b, a);
}

FrDrawSurface* fr_i_draw_create_image_surface (FrDraw *self, SysInt width, SysInt height) {
  sys_return_val_if_fail(self != NULL, NULL);

  return FR_I_DRAW_GET_IFACE(self)->create_image_surface(width, height);
}

FrDrawSurface* fr_i_draw_create_surface (FrIDevice* idevice, SysInt width, SysInt height) {
  FrIDrawInterface* iface = fr_draw_get_iface();
  sys_return_val_if_fail(iface != NULL, NULL);

  return iface->create_surface(idevice, width, height);
}

FrDrawSurface* fr_i_draw_create_image_surface_from_surface (FrDrawSurface *surface, SysInt width, SysInt height) {
  FrIDrawInterface* iface = fr_draw_get_iface();
  sys_return_val_if_fail(iface != NULL, NULL);

  return iface->create_image_surface_from_surface(surface, width, height);
}

SYS_API FrDrawContext* fr_i_draw_create_cr (FrDrawSurface *surface) {
  FrIDrawInterface* iface = fr_draw_get_iface();
  sys_return_val_if_fail(iface != NULL, NULL);

  return iface->create_cr(surface);
}

void fr_i_draw_stroke_mp (FrDrawContext* cr, const FrRect *bound, const FrSInt4* m4, const FrSInt4* p4) {
  FrIDrawInterface* iface = fr_draw_get_iface();
  sys_return_if_fail(iface != NULL);

  iface->stroke_mp(cr, bound, m4, p4);
}

void fr_i_draw_context_fill_background (FrDrawContext *cr, SysInt width, SysInt height) {
  FrIDrawInterface* iface = fr_draw_get_iface();
  sys_return_if_fail(iface != NULL);

  iface->context_fill_background(cr, width, height);
}
