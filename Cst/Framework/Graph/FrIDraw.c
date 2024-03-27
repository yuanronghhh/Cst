#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrDraw.h>

SYS_DEFINE_INTERFACE(FrIDraw, fr_i_draw, SYS_TYPE_OBJECT);

/* object api */
void fr_i_draw_default_init(FrIDrawInterface* iface) {
}

FrDrawSurface* fr_i_draw_surface_create_similar_image (FrDrawSurface* other, SysInt width, SysInt height) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surface_create_similar_image(other, width, height);
}

FrDrawSurface* fr_i_draw_image_surface_create (SysInt width, SysInt height) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->image_surface_create(width, height);
}

FrDrawSurface* fr_i_draw_create_surface (FrIDevice *device, SysInt width, SysInt height) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->create_surface(device, width, height);
}

FrDrawSurface* fr_i_draw_get_target (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->get_target(cr);
}

FrDrawSurface* fr_i_draw_image_surface_create_from_png (const char * filename) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->image_surface_create_from_png(filename);
}

FrDrawSurface* fr_i_draw_surface_create_for_rectangle (FrDrawSurface* target,SysDouble x,SysDouble y,SysDouble width,SysDouble height) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surface_create_for_rectangle(target, x, y, width, height);
}

FrDrawSurface* fr_i_draw_surface_reference (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->surface_reference(surface);
}

FrDrawContext* fr_i_draw_create (FrDrawSurface* target) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, NULL);

  return self->create(target);
}

SysDouble fr_i_draw_get_line_width (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->get_line_width(cr);
}

SysInt fr_i_draw_image_surface_get_height (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->image_surface_get_height(surface);
}

SysInt fr_i_draw_image_surface_get_stride (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->image_surface_get_stride(surface);
}

SysInt fr_i_draw_image_surface_get_width (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_val_if_fail(self != NULL, -1);

  return self->image_surface_get_width(surface);
}

void fr_i_draw_arc (FrDrawContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->arc(cr, xc, yc, radius, angle1, angle2);
}

void fr_i_draw_arc_negative (FrDrawContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->arc_negative(cr, xc, yc, radius, angle1, angle2);
}

void fr_i_draw_clip (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->clip(cr);
}

void fr_i_draw_clip_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->clip_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_clip_preserve (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->clip_preserve(cr);
}

void fr_i_draw_close_path (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->close_path(cr);
}

void fr_i_draw_curve_to (FrDrawContext* cr,SysDouble x1,SysDouble y1,SysDouble x2,SysDouble y2,SysDouble x3,SysDouble y3) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->curve_to(cr, x1, y1, x2, y2, x3, y3);
}

void fr_i_draw_destroy (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->destroy(cr);
}

void fr_i_draw_fill (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->fill(cr);
}

void fr_i_draw_fill_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->fill_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_fill_preserve (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->fill_preserve(cr);
}

void fr_i_draw_line_to (FrDrawContext* cr,SysDouble x,SysDouble y) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->line_to(cr, x, y);
}

void fr_i_draw_mask_surface (FrDrawContext* cr,FrDrawSurface* surface,SysDouble surface_x,SysDouble surface_y) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->mask_surface(cr, surface, surface_x, surface_y);
}

void fr_i_draw_move_to (FrDrawContext* cr,SysDouble x,SysDouble y) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->move_to(cr, x, y);
}

void fr_i_draw_new_path (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->new_path(cr);
}

void fr_i_draw_new_sub_path (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->new_sub_path(cr);
}

void fr_i_draw_paint (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->paint(cr);
}

void fr_i_draw_paint_with_alpha (FrDrawContext* cr,SysDouble alpha) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->paint_with_alpha(cr, alpha);
}

void fr_i_draw_path_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->path_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_pop_group_to_source (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->pop_group_to_source(cr);
}

void fr_i_draw_push_group (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->push_group(cr);
}

void fr_i_draw_recording_surface_ink_extents (FrDrawSurface* surface,SysDouble * x0,SysDouble * y0,SysDouble * width,SysDouble * height) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->recording_surface_ink_extents(surface, x0, y0, width, height);
}

void fr_i_draw_rectangle (FrDrawContext* cr,SysDouble x,SysDouble y,SysDouble width,SysDouble height) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->rectangle(cr, x, y, width, height);
}

void fr_i_draw_rel_curve_to (FrDrawContext* cr,SysDouble dx1,SysDouble dy1,SysDouble dx2,SysDouble dy2,SysDouble dx3,SysDouble dy3) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->rel_curve_to(cr, dx1, dy1, dx2, dy2, dx3, dy3);
}

void fr_i_draw_rel_line_to (FrDrawContext* cr,SysDouble dx,SysDouble dy) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->rel_line_to(cr, dx, dy);
}

void fr_i_draw_rel_move_to (FrDrawContext* cr,SysDouble dx,SysDouble dy) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->rel_move_to(cr, dx, dy);
}

void fr_i_draw_reset_clip (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->reset_clip(cr);
}

void fr_i_draw_restore (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->restore(cr);
}

void fr_i_draw_rotate (FrDrawContext* cr,SysDouble angle) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->rotate(cr, angle);
}

void fr_i_draw_save (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->save(cr);
}

void fr_i_draw_scale (FrDrawContext* cr,SysDouble sx,SysDouble sy) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->scale(cr, sx, sy);
}

void fr_i_draw_set_dash (FrDrawContext* cr,const SysDouble * dashes,SysInt num_dashes,SysDouble offset) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_dash(cr, dashes, num_dashes, offset);
}

void fr_i_draw_set_font_size (FrDrawContext* cr,SysDouble size) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_font_size(cr, size);
}

void fr_i_draw_set_line_width (FrDrawContext* cr,SysDouble width) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_line_width(cr, width);
}

void fr_i_draw_set_source_rgb (FrDrawContext* cr,SysDouble red,SysDouble green,SysDouble blue) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_source_rgb(cr, red, green, blue);
}

void fr_i_draw_set_source_rgba (FrDrawContext* cr,SysDouble red,SysDouble green,SysDouble blue,SysDouble alpha) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_source_rgba(cr, red, green, blue, alpha);
}

void fr_i_draw_set_source_surface (FrDrawContext* cr,FrDrawSurface* surface,SysDouble x,SysDouble y) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->set_source_surface(cr, surface, x, y);
}

void fr_i_draw_stroke (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->stroke(cr);
}

void fr_i_draw_stroke_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->stroke_extents(cr, x1, y1, x2, y2);
}

void fr_i_draw_stroke_preserve (FrDrawContext* cr) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->stroke_preserve(cr);
}

void fr_i_draw_surface_copy_page (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_copy_page(surface);
}

void fr_i_draw_surface_destroy (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_destroy(surface);
}

void fr_i_draw_surface_finish (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_finish(surface);
}

void fr_i_draw_surface_flush (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_flush(surface);
}

void fr_i_draw_surface_get_device_offset (FrDrawSurface* surface,SysDouble * x_offset,SysDouble * y_offset) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_device_offset(surface, x_offset, y_offset);
}

void fr_i_draw_surface_get_device_scale (FrDrawSurface* surface,SysDouble * x_scale,SysDouble * y_scale) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_device_scale(surface, x_scale, y_scale);
}

void fr_i_draw_surface_get_fallback_resolution (FrDrawSurface* surface,SysDouble * x_pixels_per_inch,SysDouble * y_pixels_per_inch) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_fallback_resolution(surface, x_pixels_per_inch, y_pixels_per_inch);
}

void fr_i_draw_surface_get_mime_data (FrDrawSurface* surface,const char * mime_type,const unsigned char ** data,unsigned long * length) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_get_mime_data(surface, mime_type, data, length);
}

void fr_i_draw_surface_mark_dirty (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_mark_dirty(surface);
}

void fr_i_draw_surface_mark_dirty_rectangle (FrDrawSurface* surface,SysInt x,SysInt y,SysInt width,SysInt height) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_mark_dirty_rectangle(surface, x, y, width, height);
}

void fr_i_draw_surface_set_device_offset (FrDrawSurface* surface,SysDouble x_offset,SysDouble y_offset) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_set_device_offset(surface, x_offset, y_offset);
}

void fr_i_draw_surface_set_device_scale (FrDrawSurface* surface,SysDouble x_scale,SysDouble y_scale) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_set_device_scale(surface, x_scale, y_scale);
}

void fr_i_draw_surface_set_fallback_resolution (FrDrawSurface* surface,SysDouble x_pixels_per_inch,SysDouble y_pixels_per_inch) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_set_fallback_resolution(surface, x_pixels_per_inch, y_pixels_per_inch);
}

void fr_i_draw_surface_show_page (FrDrawSurface* surface) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_show_page(surface);
}

void fr_i_draw_surface_unmap_image (FrDrawSurface* surface,FrDrawSurface* image) {
  FrIDrawInterface *self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->surface_unmap_image(surface, image);
}

void fr_i_draw_update_layout(FrDrawContext *cr, PangoLayout *layout) {
  FrIDrawInterface* self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->update_layout(cr, layout);
}

void fr_i_draw_show_layout(FrDrawContext* cr, PangoLayout* layout) {
  FrIDrawInterface* self = fr_draw_get_iface();
  sys_return_if_fail(self != NULL);

  self->show_layout(cr, layout);
}
