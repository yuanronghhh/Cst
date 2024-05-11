#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrDrawManager.h>

SYS_DEFINE_INTERFACE(FrIDraw, fr_i_draw, SYS_TYPE_OBJECT);

/* object api */
void fr_i_draw_default_init(FrIDrawInterface* iface) {
}

void fr_i_draw_surface_create (FrSurface* o, FrSurfaceContext *info) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_create(o, info);
}

FrSurface* fr_i_draw_surface_create_similar_image (FrSurface* other,
    SysInt width,
    SysInt height) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surface_create_similar_image(other, width, height);
}

FrSurface* fr_i_draw_get_target (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->get_target(cr);
}

FrSurface* fr_i_draw_image_surface_create_from_png (const char * filename) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->image_surface_create_from_png(filename);
}

FrSurface* fr_i_draw_surface_create_for_rectangle (FrSurface* target,
    SysDouble x,
    SysDouble y,
    SysDouble width,
    SysDouble height) {

  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surface_create_for_rectangle(target, x, y, width, height);
}

FrSurface* fr_i_draw_surface_reference (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surface_reference(surface);
}

FrContext* fr_i_draw_create (FrSurface* target) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->create(target);
}

SysDouble fr_i_draw_get_line_width (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->get_line_width(cr);
}

SysInt fr_i_draw_image_surface_get_height (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->image_surface_get_height(surface);
}

SysInt fr_i_draw_image_surface_get_stride (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->image_surface_get_stride(surface);
}

SysInt fr_i_draw_image_surface_get_width (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->image_surface_get_width(surface);
}

void fr_i_draw_arc (FrContext* cr,SysDouble xc,
    SysDouble yc,
    SysDouble radius,
    SysDouble angle1,
    SysDouble angle2) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->arc(cr, xc, yc, radius, angle1, angle2);
}

void fr_i_draw_arc_negative (FrContext* cr,
    SysDouble xc,
    SysDouble yc,
    SysDouble radius,
    SysDouble angle1,
    SysDouble angle2) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->arc_negative(cr, xc, yc, radius, angle1, angle2);
}

void fr_i_draw_clip (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->clip(cr);
}

void fr_i_draw_clip_extents (FrContext* cr,
    SysDouble * x1,
    SysDouble * y1,
    SysDouble * x2,
    SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->clip_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_clip_preserve (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->clip_preserve(cr);
}

void fr_i_draw_close_path (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->close_path(cr);
}

void fr_i_draw_curve_to (FrContext* cr,
    SysDouble x1,
    SysDouble y1,
    SysDouble x2,
    SysDouble y2,
    SysDouble x3,
    SysDouble y3) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->curve_to(cr, x1, y1, x2, y2, x3, y3);
}

void fr_i_draw_destroy (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->destroy(cr);
}

void fr_i_draw_fill (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->fill(cr);
}

void fr_i_draw_fill_extents (FrContext* cr,
    SysDouble * x1,
    SysDouble * y1,
    SysDouble * x2,
    SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->fill_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_fill_preserve (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->fill_preserve(cr);
}

void fr_i_draw_line_to (FrContext* cr,SysDouble x,SysDouble y) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->line_to(cr, x, y);
}

void fr_i_draw_mask_surface (FrContext* cr,
    FrSurface* surface,
    SysDouble surface_x,
    SysDouble surface_y) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->mask_surface(cr, surface, surface_x, surface_y);
}

void fr_i_draw_move_to (FrContext* cr,SysDouble x,SysDouble y) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->move_to(cr, x, y);
}

void fr_i_draw_new_path (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->new_path(cr);
}

void fr_i_draw_new_sub_path (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->new_sub_path(cr);
}

void fr_i_draw_paint (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->paint(cr);
}

void fr_i_draw_paint_with_alpha (FrContext* cr,SysDouble alpha) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->paint_with_alpha(cr, alpha);
}

void fr_i_draw_path_extents (FrContext* cr,
    SysDouble * x1,
    SysDouble * y1,
    SysDouble * x2,
    SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->path_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_pop_group_to_source (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->pop_group_to_source(cr);
}

void fr_i_draw_push_group (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->push_group(cr);
}

void fr_i_draw_recording_surface_ink_extents (FrSurface* surface,
    SysDouble * x0,
    SysDouble * y0,
    SysDouble * width,
    SysDouble * height) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->recording_surface_ink_extents(surface, x0, y0, width, height);
}

void fr_i_draw_rectangle (FrContext* cr,
    SysDouble x,
    SysDouble y,
    SysDouble width,
    SysDouble height) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->rectangle(cr, x, y, width, height);
}

void fr_i_draw_rel_curve_to (FrContext* cr,
    SysDouble dx1,
    SysDouble dy1,
    SysDouble dx2,
    SysDouble dy2,
    SysDouble dx3,
    SysDouble dy3) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->rel_curve_to(cr, dx1, dy1, dx2, dy2, dx3, dy3);
}

void fr_i_draw_rel_line_to (FrContext* cr,SysDouble dx,SysDouble dy) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->rel_line_to(cr, dx, dy);
}

void fr_i_draw_rel_move_to (FrContext* cr,SysDouble dx,SysDouble dy) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->rel_move_to(cr, dx, dy);
}

void fr_i_draw_reset_clip (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->reset_clip(cr);
}

void fr_i_draw_restore (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->restore(cr);
}

void fr_i_draw_rotate (FrContext* cr,SysDouble angle) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->rotate(cr, angle);
}

void fr_i_draw_save (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->save(cr);
}

void fr_i_draw_scale (FrContext* cr,SysDouble sx,SysDouble sy) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->scale(cr, sx, sy);
}

void fr_i_draw_set_dash (FrContext* cr,
    const SysDouble * dashes,
    SysInt num_dashes,
    SysDouble offset) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_dash(cr, dashes, num_dashes, offset);
}

void fr_i_draw_set_font_size (FrContext* cr,SysDouble size) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_font_size(cr, size);
}

void fr_i_draw_set_line_width (FrContext* cr,SysDouble width) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_line_width(cr, width);
}

void fr_i_draw_set_color (FrContext* cr, FrColor *color) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_color(cr, color);
}

void fr_i_draw_set_source_surface (FrContext* cr,
    FrSurface* surface,
    SysDouble x,
    SysDouble y) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_source_surface(cr, surface, x, y);
}

void fr_i_draw_stroke (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->stroke(cr);
}

void fr_i_draw_stroke_extents (FrContext* cr,
    SysDouble * x1,
    SysDouble * y1,
    SysDouble * x2,
    SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->stroke_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_stroke_preserve (FrContext* cr) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->stroke_preserve(cr);
}

void fr_i_draw_surface_copy_page (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_copy_page(surface);
}

void fr_i_draw_surface_destroy (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_destroy(surface);
}

void fr_i_draw_surface_finish (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_finish(surface);
}

void fr_i_draw_surface_flush (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_flush(surface);
}

void fr_i_draw_surface_get_device_offset (FrSurface* surface,
    SysDouble * x_offset,
    SysDouble * y_offset) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_device_offset(surface, x_offset, y_offset);
}

void fr_i_draw_surface_get_device_scale (FrSurface* surface,
    SysDouble * x_scale,
    SysDouble * y_scale) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_device_scale(surface, x_scale, y_scale);
}

void fr_i_draw_surface_get_fallback_resolution (FrSurface* surface,
    SysDouble * x_pixels_per_inch,
    SysDouble * y_pixels_per_inch) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_fallback_resolution(surface,
      x_pixels_per_inch, 
      y_pixels_per_inch);
}

void fr_i_draw_surface_get_mime_data (FrSurface* surface,
    const char * mime_type,
    const unsigned char ** data,
    unsigned long * length) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_mime_data(surface, mime_type, data, length);
}

void fr_i_draw_surface_mark_dirty (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_mark_dirty(surface);
}

void fr_i_draw_surface_mark_dirty_rectangle (FrSurface* surface,
    SysInt x,
    SysInt y,
    SysInt width,
    SysInt height) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_mark_dirty_rectangle(surface, x, y, width, height);
}

void fr_i_draw_surface_set_device_offset (FrSurface* surface,
    SysDouble x_offset,
    SysDouble y_offset) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_set_device_offset(surface, x_offset, y_offset);
}

void fr_i_draw_surface_set_device_scale (FrSurface* surface,
    SysDouble x_scale,
    SysDouble y_scale) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_set_device_scale(surface, x_scale, y_scale);
}

void fr_i_draw_surface_set_fallback_resolution (FrSurface* surface,
    SysDouble x_pixels_per_inch,
    SysDouble y_pixels_per_inch) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_set_fallback_resolution(surface,
      x_pixels_per_inch, 
      y_pixels_per_inch);
}

void fr_i_draw_surface_show_page (FrSurface* surface) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_show_page(surface);
}

void fr_i_draw_surface_unmap_image (FrSurface* surface,FrSurface* image) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_unmap_image(surface, image);
}

void fr_i_draw_update_layout(FrContext *cr, PangoLayout *layout) {
  FrIDrawInterface* self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->update_layout(cr, layout);
}

void fr_i_draw_show_layout(FrContext* cr, PangoLayout* layout) {
  FrIDrawInterface* self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->show_layout(cr, layout);
}

SysInt fr_i_draw_rounded_rectangle(FrContext* cr,
    SysDouble x,
    SysDouble y,
    SysDouble w,
    SysDouble h,
    SysDouble radius) {
  FrIDrawInterface* self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->rounded_rectangle(cr, x, y, w, h, radius);
}

void fr_i_draw_context_overlay (FrContext* cr,
    FrSurface *surface,
    SysInt x,
    SysInt y) {
  FrIDrawInterface* self = fr_draw_manager_get_iface();
  sys_return_if_fail(self != NULL);

  self->overlay(cr, surface, x, y);
}

void fr_i_draw_show_text (FrContext* cr, const SysChar *text) {
  FrIDrawInterface* self = fr_draw_manager_get_iface();

  sys_return_if_fail(self != NULL);

  self->show_text(cr, text);
}

FrSurface* fr_i_draw_resize_surface (FrSurface *surface, FrSurface *device_surface, SysInt width, SysInt height) {
  FrIDrawInterface *self = fr_draw_manager_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->resize_surface(surface, device_surface, width, height);
}
