#ifndef __FR_I_DRAW__
#define __FR_I_DRAW__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_DRAW (fr_i_draw_get_type())
#define FR_I_DRAW(o) ((FrIDraw* )sys_object_cast_check(o, FR_TYPE_I_DRAW))
#define FR_I_DRAW_GET_IFACE(o) ((FrIDrawInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_DRAW))

struct _FrIDrawInterface {
  SysTypeInterface parent;

  SysInt (*rounded_rectangle) (FrDrawBrush* cr, SysDouble x, SysDouble y, SysDouble w, SysDouble h, SysDouble radius);
  FrDrawSurface* (*surface_create_similar_image) (FrDrawSurface* other, SysInt width, SysInt height);
  FrDrawSurface* (*image_surface_create) (SysInt width, SysInt height);
  FrDrawSurface* (*create_surface) (FrIDevice *device, SysInt width, SysInt height);
  FrDrawSurface* (*get_target) (FrDrawBrush* cr);
  FrDrawSurface* (*image_surface_create_from_png) (const char * filename);
  FrDrawSurface* (*surface_create_for_rectangle) (FrDrawSurface* target,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
  FrDrawSurface* (*surface_reference) (FrDrawSurface* surface);
  FrDrawBrush* (*create) (FrDrawSurface* target);
  SysDouble (*get_line_width) (FrDrawBrush* cr);
  SysInt (*image_surface_get_height) (FrDrawSurface* surface);
  SysInt (*image_surface_get_stride) (FrDrawSurface* surface);
  SysInt (*image_surface_get_width) (FrDrawSurface* surface);
  void (*arc) (FrDrawBrush* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
  void (*arc_negative) (FrDrawBrush* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
  void (*clip) (FrDrawBrush* cr);
  void (*clip_extents) (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*clip_preserve) (FrDrawBrush* cr);
  void (*close_path) (FrDrawBrush* cr);
  void (*curve_to) (FrDrawBrush* cr,SysDouble x1,SysDouble y1,SysDouble x2,SysDouble y2,SysDouble x3,SysDouble y3);
  void (*destroy) (FrDrawBrush* cr);
  void (*fill) (FrDrawBrush* cr);
  void (*fill_extents) (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*fill_preserve) (FrDrawBrush* cr);
  void (*line_to) (FrDrawBrush* cr,SysDouble x,SysDouble y);
  void (*mask_surface) (FrDrawBrush* cr,FrDrawSurface* surface,SysDouble surface_x,SysDouble surface_y);
  void (*move_to) (FrDrawBrush* cr,SysDouble x,SysDouble y);
  void (*new_path) (FrDrawBrush* cr);
  void (*new_sub_path) (FrDrawBrush* cr);
  void (*paint) (FrDrawBrush* cr);
  void (*paint_with_alpha) (FrDrawBrush* cr,SysDouble alpha);
  void (*path_extents) (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*pop_group_to_source) (FrDrawBrush* cr);
  void (*push_group) (FrDrawBrush* cr);
  void (*recording_surface_ink_extents) (FrDrawSurface* surface,SysDouble * x0,SysDouble * y0,SysDouble * width,SysDouble * height);
  void (*rectangle) (FrDrawBrush* cr,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
  void (*rel_curve_to) (FrDrawBrush* cr,SysDouble dx1,SysDouble dy1,SysDouble dx2,SysDouble dy2,SysDouble dx3,SysDouble dy3);
  void (*rel_line_to) (FrDrawBrush* cr,SysDouble dx,SysDouble dy);
  void (*rel_move_to) (FrDrawBrush* cr,SysDouble dx,SysDouble dy);
  void (*reset_clip) (FrDrawBrush* cr);
  void (*restore) (FrDrawBrush* cr);
  void (*rotate) (FrDrawBrush* cr,SysDouble angle);
  void (*save) (FrDrawBrush* cr);
  void (*scale) (FrDrawBrush* cr,SysDouble sx,SysDouble sy);
  void (*set_dash) (FrDrawBrush* cr,const SysDouble * dashes,SysInt num_dashes,SysDouble offset);
  void (*set_font_size) (FrDrawBrush* cr,SysDouble size);
  void (*set_line_width) (FrDrawBrush* cr,SysDouble width);
  void (*set_source_rgb) (FrDrawBrush* cr,SysDouble red,SysDouble green,SysDouble blue);
  void (*set_source_rgba) (FrDrawBrush* cr,SysDouble red,SysDouble green,SysDouble blue,SysDouble alpha);
  void (*set_source_surface) (FrDrawBrush* cr,FrDrawSurface* surface,SysDouble x,SysDouble y);
  void (*stroke) (FrDrawBrush* cr);
  void (*stroke_extents) (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*stroke_preserve) (FrDrawBrush* cr);
  void (*surface_copy_page) (FrDrawSurface* surface);
  void (*surface_destroy) (FrDrawSurface* surface);
  void (*surface_finish) (FrDrawSurface* surface);
  void (*surface_flush) (FrDrawSurface* surface);
  void (*surface_get_device_offset) (FrDrawSurface* surface,SysDouble * x_offset,SysDouble * y_offset);
  void (*surface_get_device_scale) (FrDrawSurface* surface,SysDouble * x_scale,SysDouble * y_scale);
  void (*surface_get_fallback_resolution) (FrDrawSurface* surface,SysDouble * x_pixels_per_inch,SysDouble * y_pixels_per_inch);
  void (*surface_get_mime_data) (FrDrawSurface* surface,const char * mime_type,const unsigned char ** data,unsigned long * length);
  void (*surface_mark_dirty) (FrDrawSurface* surface);
  void (*surface_mark_dirty_rectangle) (FrDrawSurface* surface,SysInt x,SysInt y,SysInt width,SysInt height);
  void (*surface_set_device_offset) (FrDrawSurface* surface,SysDouble x_offset,SysDouble y_offset);
  void (*surface_set_device_scale) (FrDrawSurface* surface,SysDouble x_scale,SysDouble y_scale);
  void (*surface_set_fallback_resolution) (FrDrawSurface* surface,SysDouble x_pixels_per_inch,SysDouble y_pixels_per_inch);
  void (*surface_show_page) (FrDrawSurface* surface);
  void (*surface_unmap_image) (FrDrawSurface* surface,FrDrawSurface* image);
  void (*update_layout) (FrDrawBrush* cr, PangoLayout* layout);
  void (*show_layout) (FrDrawBrush* cr, PangoLayout* layout);
  void (*overlay) (FrDrawBrush* cr, FrDrawSurface *surface, SysInt x, SysInt y);
};


SysType fr_i_draw_get_type(void);


FrDrawSurface* fr_i_draw_surface_create_similar_image (FrDrawSurface* other, SysInt width, SysInt height);
FrDrawSurface* fr_i_draw_image_surface_create (SysInt width, SysInt height);
FrDrawSurface* fr_i_draw_create_surface (FrIDevice *device, SysInt width, SysInt height);
FrDrawSurface* fr_i_draw_get_target (FrDrawBrush* cr);
FrDrawSurface* fr_i_draw_image_surface_create_from_png (const char * filename);
FrDrawSurface* fr_i_draw_surface_create_for_rectangle (FrDrawSurface* target,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
FrDrawSurface* fr_i_draw_surface_reference (FrDrawSurface* surface);
FrDrawBrush* fr_i_draw_create (FrDrawSurface* target);
SysDouble fr_i_draw_get_line_width (FrDrawBrush* cr);
SysInt fr_i_draw_image_surface_get_height (FrDrawSurface* surface);
SysInt fr_i_draw_image_surface_get_stride (FrDrawSurface* surface);
SysInt fr_i_draw_image_surface_get_width (FrDrawSurface* surface);
void fr_i_draw_arc (FrDrawBrush* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
void fr_i_draw_arc_negative (FrDrawBrush* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
void fr_i_draw_clip (FrDrawBrush* cr);
void fr_i_draw_clip_extents (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_clip_preserve (FrDrawBrush* cr);
void fr_i_draw_close_path (FrDrawBrush* cr);
void fr_i_draw_curve_to (FrDrawBrush* cr,SysDouble x1,SysDouble y1,SysDouble x2,SysDouble y2,SysDouble x3,SysDouble y3);
void fr_i_draw_destroy (FrDrawBrush* cr);
void fr_i_draw_fill (FrDrawBrush* cr);
void fr_i_draw_fill_extents (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_fill_preserve (FrDrawBrush* cr);
void fr_i_draw_line_to (FrDrawBrush* cr,SysDouble x,SysDouble y);
void fr_i_draw_mask_surface (FrDrawBrush* cr,FrDrawSurface* surface,SysDouble surface_x,SysDouble surface_y);
void fr_i_draw_move_to (FrDrawBrush* cr,SysDouble x,SysDouble y);
void fr_i_draw_new_path (FrDrawBrush* cr);
void fr_i_draw_new_sub_path (FrDrawBrush* cr);
void fr_i_draw_paint (FrDrawBrush* cr);
void fr_i_draw_paint_with_alpha (FrDrawBrush* cr,SysDouble alpha);
void fr_i_draw_path_extents (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_pop_group_to_source (FrDrawBrush* cr);
void fr_i_draw_push_group (FrDrawBrush* cr);
void fr_i_draw_recording_surface_ink_extents (FrDrawSurface* surface,SysDouble * x0,SysDouble * y0,SysDouble * width,SysDouble * height);
void fr_i_draw_rectangle (FrDrawBrush* cr,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
void fr_i_draw_rel_curve_to (FrDrawBrush* cr,SysDouble dx1,SysDouble dy1,SysDouble dx2,SysDouble dy2,SysDouble dx3,SysDouble dy3);
void fr_i_draw_rel_line_to (FrDrawBrush* cr,SysDouble dx,SysDouble dy);
void fr_i_draw_rel_move_to (FrDrawBrush* cr,SysDouble dx,SysDouble dy);
void fr_i_draw_reset_clip (FrDrawBrush* cr);
void fr_i_draw_restore (FrDrawBrush* cr);
void fr_i_draw_rotate (FrDrawBrush* cr,SysDouble angle);
void fr_i_draw_save (FrDrawBrush* cr);
void fr_i_draw_scale (FrDrawBrush* cr,SysDouble sx,SysDouble sy);
void fr_i_draw_set_dash (FrDrawBrush* cr,const SysDouble * dashes,SysInt num_dashes,SysDouble offset);
void fr_i_draw_set_font_size (FrDrawBrush* cr,SysDouble size);
void fr_i_draw_set_line_width (FrDrawBrush* cr,SysDouble width);
void fr_i_draw_set_source_rgb (FrDrawBrush* cr,SysDouble red,SysDouble green,SysDouble blue);
void fr_i_draw_set_source_rgba (FrDrawBrush* cr,SysDouble red,SysDouble green,SysDouble blue,SysDouble alpha);
void fr_i_draw_set_source_surface (FrDrawBrush* cr,FrDrawSurface* surface,SysDouble x,SysDouble y);
void fr_i_draw_stroke (FrDrawBrush* cr);
void fr_i_draw_stroke_extents (FrDrawBrush* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_stroke_preserve (FrDrawBrush* cr);
void fr_i_draw_surface_copy_page (FrDrawSurface* surface);
void fr_i_draw_surface_destroy (FrDrawSurface* surface);
void fr_i_draw_surface_finish (FrDrawSurface* surface);
void fr_i_draw_surface_flush (FrDrawSurface* surface);
void fr_i_draw_surface_get_device_offset (FrDrawSurface* surface,SysDouble * x_offset,SysDouble * y_offset);
void fr_i_draw_surface_get_device_scale (FrDrawSurface* surface,SysDouble * x_scale,SysDouble * y_scale);
void fr_i_draw_surface_get_fallback_resolution (FrDrawSurface* surface,SysDouble * x_pixels_per_inch,SysDouble * y_pixels_per_inch);
void fr_i_draw_surface_get_mime_data (FrDrawSurface* surface,const char * mime_type,const unsigned char ** data,unsigned long * length);
void fr_i_draw_surface_mark_dirty (FrDrawSurface* surface);
void fr_i_draw_surface_mark_dirty_rectangle (FrDrawSurface* surface,SysInt x,SysInt y,SysInt width,SysInt height);
void fr_i_draw_surface_set_device_offset (FrDrawSurface* surface,SysDouble x_offset,SysDouble y_offset);
void fr_i_draw_surface_set_device_scale (FrDrawSurface* surface,SysDouble x_scale,SysDouble y_scale);
void fr_i_draw_surface_set_fallback_resolution (FrDrawSurface* surface,SysDouble x_pixels_per_inch,SysDouble y_pixels_per_inch);
void fr_i_draw_surface_show_page (FrDrawSurface* surface);
void fr_i_draw_surface_unmap_image (FrDrawSurface* surface,FrDrawSurface* image);
void fr_i_draw_update_layout(FrDrawBrush* cr, PangoLayout* layout);
void fr_i_draw_show_layout(FrDrawBrush* cr, PangoLayout* layout);
SysInt fr_i_draw_rounded_rectangle(FrDrawBrush* cr, SysDouble x, SysDouble y, SysDouble w, SysDouble h, SysDouble radius);
void fr_i_draw_context_overlay (FrDrawBrush* cr, FrDrawSurface *surface, SysInt x, SysInt y);

SYS_END_DECLS

#endif
