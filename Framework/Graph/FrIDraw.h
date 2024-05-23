#ifndef __FR_I_DRAW__
#define __FR_I_DRAW__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_DRAW (fr_i_draw_get_type())
#define FR_I_DRAW(o) ((FrIDraw* )sys_object_cast_check(o, FR_TYPE_I_DRAW))
#define FR_I_DRAW_GET_IFACE(o) ((FrIDrawInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_DRAW))

struct _FrIDrawInterface {
  SysTypeInterface parent;

  FrSurface *(*resize_surface)(FrSurface *self, FrSurface *device_surface, SysInt width, SysInt height);
  SysInt (*rounded_rectangle) (FrContext* cr, SysDouble x, SysDouble y, SysDouble w, SysDouble h, SysDouble radius);
  void (*surface_create) (FrSurface* o, FrSurfaceContext *info);
  FrSurface* (*surface_create_similar_image) (FrSurface* other, SysInt width, SysInt height);
  FrSurface* (*get_target) (FrContext* cr);
  FrSurface* (*image_surface_create_from_png) (const char * filename);
  FrSurface* (*surface_create_for_rectangle) (FrSurface* target,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
  FrSurface* (*surface_reference) (FrSurface* surface);
  void (*create) (FrContext *cr, FrSurface* surface);
  SysDouble (*get_line_width) (FrContext* cr);
  SysInt (*image_surface_get_height) (FrSurface* surface);
  SysInt (*image_surface_get_stride) (FrSurface* surface);
  SysInt (*image_surface_get_width) (FrSurface* surface);
  void (*arc) (FrContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
  void (*arc_negative) (FrContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
  void (*clip) (FrContext* cr);
  void (*clip_extents) (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*clip_preserve) (FrContext* cr);
  void (*close_path) (FrContext* cr);
  void (*curve_to) (FrContext* cr,SysDouble x1,SysDouble y1,SysDouble x2,SysDouble y2,SysDouble x3,SysDouble y3);
  void (*destroy) (FrContext* cr);
  void (*fill) (FrContext* cr);
  void (*fill_extents) (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*fill_preserve) (FrContext* cr);
  void (*line_to) (FrContext* cr,SysDouble x,SysDouble y);
  void (*mask_surface) (FrContext* cr,FrSurface* surface,SysDouble surface_x,SysDouble surface_y);
  void (*move_to) (FrContext* cr,SysDouble x,SysDouble y);
  void (*new_path) (FrContext* cr);
  void (*new_sub_path) (FrContext* cr);
  void (*paint) (FrContext* cr);
  void (*paint_with_alpha) (FrContext* cr,SysDouble alpha);
  void (*path_extents) (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*pop_group_to_source) (FrContext* cr);
  void (*push_group) (FrContext* cr);
  void (*recording_surface_ink_extents) (FrSurface* surface,SysDouble * x0,SysDouble * y0,SysDouble * width,SysDouble * height);
  void (*rectangle) (FrContext* cr,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
  void (*rel_curve_to) (FrContext* cr,SysDouble dx1,SysDouble dy1,SysDouble dx2,SysDouble dy2,SysDouble dx3,SysDouble dy3);
  void (*rel_line_to) (FrContext* cr,SysDouble dx,SysDouble dy);
  void (*rel_move_to) (FrContext* cr,SysDouble dx,SysDouble dy);
  void (*reset_clip) (FrContext* cr);
  void (*restore) (FrContext* cr);
  void (*rotate) (FrContext* cr,SysDouble angle);
  void (*save) (FrContext* cr);
  void (*scale) (FrContext* cr,SysDouble sx,SysDouble sy);
  void (*set_dash) (FrContext* cr,const SysDouble * dashes,SysInt num_dashes,SysDouble offset);
  void (*set_font_size) (FrContext* cr,SysDouble size);
  void (*set_line_width) (FrContext* cr,SysDouble width);
  void (*set_source_rgb) (FrContext* cr,SysDouble red,SysDouble green,SysDouble blue);
  void (*set_color) (FrContext* cr, FrColor *color);
  void (*set_source_surface) (FrContext* cr,FrSurface* surface,SysDouble x,SysDouble y);
  void (*stroke) (FrContext* cr);
  void (*stroke_extents) (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*stroke_preserve) (FrContext* cr);
  void (*surface_copy_page) (FrSurface* surface);
  void (*surface_destroy) (FrSurface* surface);
  void (*surface_finish) (FrSurface* surface);
  void (*surface_flush) (FrSurface* surface);
  void (*surface_get_device_offset) (FrSurface* surface,SysDouble * x_offset,SysDouble * y_offset);
  void (*surface_get_device_scale) (FrSurface* surface,SysDouble * x_scale,SysDouble * y_scale);
  void (*surface_get_fallback_resolution) (FrSurface* surface,SysDouble * x_pixels_per_inch,SysDouble * y_pixels_per_inch);
  void (*surface_get_mime_data) (FrSurface* surface,const char * mime_type,const unsigned char ** data,unsigned long * length);
  void (*surface_mark_dirty) (FrSurface* surface);
  void (*surface_mark_dirty_rectangle) (FrSurface* surface,SysInt x,SysInt y,SysInt width,SysInt height);
  void (*surface_set_device_offset) (FrSurface* surface,SysDouble x_offset,SysDouble y_offset);
  void (*surface_set_device_scale) (FrSurface* surface,SysDouble x_scale,SysDouble y_scale);
  void (*surface_set_fallback_resolution) (FrSurface* surface,SysDouble x_pixels_per_inch,SysDouble y_pixels_per_inch);
  void (*surface_show_page) (FrSurface* surface);
  void (*surface_unmap_image) (FrSurface* surface,FrSurface* image);
  void (*update_layout) (FrContext* cr, PangoLayout* layout);
  void (*overlay) (FrContext* cr, FrSurface *surface, SysInt x, SysInt y);
  void (*show_layout) (FrContext* cr, PangoLayout* layout);
  void (*show_text) (FrContext* cr, const SysChar *text);
};

SysType fr_i_draw_get_type(void);
void fr_i_draw_setup(FrIDraw *draw);
void fr_i_draw_teardown(void);
FrIDrawInterface* fr_i_draw_get_g_iface(void);

void fr_i_draw_surface_create (FrSurface* o, FrSurfaceContext *info);
FrSurface* fr_i_draw_surface_create_similar_image (FrSurface* other, SysInt width, SysInt height);
FrSurface* fr_i_draw_get_target (FrContext* cr);
FrSurface* fr_i_draw_image_surface_create_from_png (const char * filename);
FrSurface* fr_i_draw_surface_create_for_rectangle (FrSurface* target,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
FrSurface* fr_i_draw_surface_reference (FrSurface* surface);
void fr_i_draw_create(FrContext* cr, FrSurface* target);
SysDouble fr_i_draw_get_line_width (FrContext* cr);
SysInt fr_i_draw_image_surface_get_height (FrSurface* surface);
SysInt fr_i_draw_image_surface_get_stride (FrSurface* surface);
SysInt fr_i_draw_image_surface_get_width (FrSurface* surface);
void fr_i_draw_arc (FrContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
void fr_i_draw_arc_negative (FrContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
void fr_i_draw_clip (FrContext* cr);
void fr_i_draw_clip_extents (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_clip_preserve (FrContext* cr);
void fr_i_draw_close_path (FrContext* cr);
void fr_i_draw_curve_to (FrContext* cr,SysDouble x1,SysDouble y1,SysDouble x2,SysDouble y2,SysDouble x3,SysDouble y3);
void fr_i_draw_destroy (FrContext* cr);
void fr_i_draw_fill (FrContext* cr);
void fr_i_draw_fill_extents (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_fill_preserve (FrContext* cr);
void fr_i_draw_line_to (FrContext* cr,SysDouble x,SysDouble y);
void fr_i_draw_mask_surface (FrContext* cr,FrSurface* surface,SysDouble surface_x,SysDouble surface_y);
void fr_i_draw_move_to (FrContext* cr,SysDouble x,SysDouble y);
void fr_i_draw_new_path (FrContext* cr);
void fr_i_draw_new_sub_path (FrContext* cr);
void fr_i_draw_paint (FrContext* cr);
void fr_i_draw_paint_with_alpha (FrContext* cr,SysDouble alpha);
void fr_i_draw_path_extents (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_pop_group_to_source (FrContext* cr);
void fr_i_draw_push_group (FrContext* cr);
void fr_i_draw_recording_surface_ink_extents (FrSurface* surface,SysDouble * x0,SysDouble * y0,SysDouble * width,SysDouble * height);
void fr_i_draw_rectangle (FrContext* cr,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
void fr_i_draw_rel_curve_to (FrContext* cr,SysDouble dx1,SysDouble dy1,SysDouble dx2,SysDouble dy2,SysDouble dx3,SysDouble dy3);
void fr_i_draw_rel_line_to (FrContext* cr,SysDouble dx,SysDouble dy);
void fr_i_draw_rel_move_to (FrContext* cr,SysDouble dx,SysDouble dy);
void fr_i_draw_reset_clip (FrContext* cr);
void fr_i_draw_restore (FrContext* cr);
void fr_i_draw_rotate (FrContext* cr,SysDouble angle);
void fr_i_draw_save (FrContext* cr);
void fr_i_draw_scale (FrContext* cr,SysDouble sx,SysDouble sy);
void fr_i_draw_set_dash (FrContext* cr,const SysDouble * dashes,SysInt num_dashes,SysDouble offset);
void fr_i_draw_set_font_size (FrContext* cr,SysDouble size);
void fr_i_draw_set_line_width (FrContext* cr,SysDouble width);
void fr_i_draw_set_color (FrContext* cr, FrColor *color);
void fr_i_draw_set_source_surface (FrContext* cr,FrSurface* surface,SysDouble x,SysDouble y);
void fr_i_draw_stroke (FrContext* cr);
void fr_i_draw_stroke_extents (FrContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_stroke_preserve (FrContext* cr);
void fr_i_draw_surface_copy_page (FrSurface* surface);
void fr_i_draw_surface_destroy (FrSurface* surface);
void fr_i_draw_surface_finish (FrSurface* surface);
void fr_i_draw_surface_flush (FrSurface* surface);
void fr_i_draw_surface_get_device_offset (FrSurface* surface,SysDouble * x_offset,SysDouble * y_offset);
void fr_i_draw_surface_get_device_scale (FrSurface* surface,SysDouble * x_scale,SysDouble * y_scale);
void fr_i_draw_surface_get_fallback_resolution (FrSurface* surface,SysDouble * x_pixels_per_inch,SysDouble * y_pixels_per_inch);
void fr_i_draw_surface_get_mime_data (FrSurface* surface,const char * mime_type,const unsigned char ** data,unsigned long * length);
void fr_i_draw_surface_mark_dirty (FrSurface* surface);
void fr_i_draw_surface_mark_dirty_rectangle (FrSurface* surface,SysInt x,SysInt y,SysInt width,SysInt height);
void fr_i_draw_surface_set_device_offset (FrSurface* surface,SysDouble x_offset,SysDouble y_offset);
void fr_i_draw_surface_set_device_scale (FrSurface* surface,SysDouble x_scale,SysDouble y_scale);
void fr_i_draw_surface_set_fallback_resolution (FrSurface* surface,SysDouble x_pixels_per_inch,SysDouble y_pixels_per_inch);
void fr_i_draw_surface_show_page (FrSurface* surface);
void fr_i_draw_surface_unmap_image (FrSurface* surface,FrSurface* image);
void fr_i_draw_update_layout(FrContext* cr, PangoLayout* layout);
void fr_i_draw_show_layout(FrContext* cr, PangoLayout* layout);
SysInt fr_i_draw_rounded_rectangle(FrContext* cr, SysDouble x, SysDouble y, SysDouble w, SysDouble h, SysDouble radius);
void fr_i_draw_context_overlay (FrContext* cr, FrSurface *surface, SysInt x, SysInt y);
void fr_i_draw_show_text (FrContext* cr, const SysChar *text);
FrSurface* fr_i_draw_resize_surface (FrSurface *surface, FrSurface *device_surface, SysInt width, SysInt height);

SYS_END_DECLS

#endif
