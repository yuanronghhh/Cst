#ifndef __FR_I_DRAW__
#define __FR_I_DRAW__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_DRAW (fr_i_draw_get_type())
#define FR_I_DRAW(o) ((FrIDraw* )sys_object_cast_check(o, FR_TYPE_I_DRAW))
#define FR_I_DRAW_GET_IFACE(o) ((FrIDrawInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_DRAW))

struct _FrIDrawInterface {
  SysTypeInterface parent;

  FrDrawSurface* (*surface_create_similar_image) (FrDrawSurface* other, SysInt width, SysInt height);
  FrDrawSurface* (*image_surface_create) (SysInt width, SysInt height);
  FrDrawSurface* (*create_surface) (FrIDevice *device, SysInt width, SysInt height);
  FrDrawSurface* (*get_target) (FrDrawContext* cr);
  FrDrawSurface* (*image_surface_create_from_png) (const char * filename);
  FrDrawSurface* (*surface_create_for_rectangle) (FrDrawSurface* target,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
  FrDrawSurface* (*surface_reference) (FrDrawSurface* surface);
  FrDrawContext* (*create) (FrDrawSurface* target);
  SysDouble (*get_line_width) (FrDrawContext* cr);
  SysInt (*image_surface_get_height) (FrDrawSurface* surface);
  SysInt (*image_surface_get_stride) (FrDrawSurface* surface);
  SysInt (*image_surface_get_width) (FrDrawSurface* surface);
  void (*arc) (FrDrawContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
  void (*arc_negative) (FrDrawContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
  void (*clip) (FrDrawContext* cr);
  void (*clip_extents) (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*clip_preserve) (FrDrawContext* cr);
  void (*close_path) (FrDrawContext* cr);
  void (*curve_to) (FrDrawContext* cr,SysDouble x1,SysDouble y1,SysDouble x2,SysDouble y2,SysDouble x3,SysDouble y3);
  void (*destroy) (FrDrawContext* cr);
  void (*fill) (FrDrawContext* cr);
  void (*fill_extents) (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*fill_preserve) (FrDrawContext* cr);
  void (*line_to) (FrDrawContext* cr,SysDouble x,SysDouble y);
  void (*mask_surface) (FrDrawContext* cr,FrDrawSurface* surface,SysDouble surface_x,SysDouble surface_y);
  void (*move_to) (FrDrawContext* cr,SysDouble x,SysDouble y);
  void (*new_path) (FrDrawContext* cr);
  void (*new_sub_path) (FrDrawContext* cr);
  void (*paint) (FrDrawContext* cr);
  void (*paint_with_alpha) (FrDrawContext* cr,SysDouble alpha);
  void (*path_extents) (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*pop_group_to_source) (FrDrawContext* cr);
  void (*push_group) (FrDrawContext* cr);
  void (*recording_surface_ink_extents) (FrDrawSurface* surface,SysDouble * x0,SysDouble * y0,SysDouble * width,SysDouble * height);
  void (*rectangle) (FrDrawContext* cr,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
  void (*rel_curve_to) (FrDrawContext* cr,SysDouble dx1,SysDouble dy1,SysDouble dx2,SysDouble dy2,SysDouble dx3,SysDouble dy3);
  void (*rel_line_to) (FrDrawContext* cr,SysDouble dx,SysDouble dy);
  void (*rel_move_to) (FrDrawContext* cr,SysDouble dx,SysDouble dy);
  void (*reset_clip) (FrDrawContext* cr);
  void (*restore) (FrDrawContext* cr);
  void (*rotate) (FrDrawContext* cr,SysDouble angle);
  void (*save) (FrDrawContext* cr);
  void (*scale) (FrDrawContext* cr,SysDouble sx,SysDouble sy);
  void (*set_dash) (FrDrawContext* cr,const SysDouble * dashes,SysInt num_dashes,SysDouble offset);
  void (*set_font_size) (FrDrawContext* cr,SysDouble size);
  void (*set_line_width) (FrDrawContext* cr,SysDouble width);
  void (*set_source_rgb) (FrDrawContext* cr,SysDouble red,SysDouble green,SysDouble blue);
  void (*set_source_rgba) (FrDrawContext* cr,SysDouble red,SysDouble green,SysDouble blue,SysDouble alpha);
  void (*set_source_surface) (FrDrawContext* cr,FrDrawSurface* surface,SysDouble x,SysDouble y);
  void (*stroke) (FrDrawContext* cr);
  void (*stroke_extents) (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
  void (*stroke_preserve) (FrDrawContext* cr);
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
  void (*update_layout) (FrDrawContext* cr, PangoLayout* layout);
  void (*show_layout) (FrDrawContext* cr, PangoLayout* layout);
};


SysType fr_i_draw_get_type(void);


FrDrawSurface* fr_i_draw_surface_create_similar_image (FrDrawSurface* other, SysInt width, SysInt height);
FrDrawSurface* fr_i_draw_image_surface_create (SysInt width, SysInt height);
FrDrawSurface* fr_i_draw_create_surface (FrIDevice *device, SysInt width, SysInt height);
FrDrawSurface* fr_i_draw_get_target (FrDrawContext* cr);
FrDrawSurface* fr_i_draw_image_surface_create_from_png (const char * filename);
FrDrawSurface* fr_i_draw_surface_create_for_rectangle (FrDrawSurface* target,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
FrDrawSurface* fr_i_draw_surface_reference (FrDrawSurface* surface);
FrDrawContext* fr_i_draw_create (FrDrawSurface* target);
SysDouble fr_i_draw_get_line_width (FrDrawContext* cr);
SysInt fr_i_draw_image_surface_get_height (FrDrawSurface* surface);
SysInt fr_i_draw_image_surface_get_stride (FrDrawSurface* surface);
SysInt fr_i_draw_image_surface_get_width (FrDrawSurface* surface);
void fr_i_draw_arc (FrDrawContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
void fr_i_draw_arc_negative (FrDrawContext* cr,SysDouble xc,SysDouble yc,SysDouble radius,SysDouble angle1,SysDouble angle2);
void fr_i_draw_clip (FrDrawContext* cr);
void fr_i_draw_clip_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_clip_preserve (FrDrawContext* cr);
void fr_i_draw_close_path (FrDrawContext* cr);
void fr_i_draw_curve_to (FrDrawContext* cr,SysDouble x1,SysDouble y1,SysDouble x2,SysDouble y2,SysDouble x3,SysDouble y3);
void fr_i_draw_destroy (FrDrawContext* cr);
void fr_i_draw_fill (FrDrawContext* cr);
void fr_i_draw_fill_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_fill_preserve (FrDrawContext* cr);
void fr_i_draw_line_to (FrDrawContext* cr,SysDouble x,SysDouble y);
void fr_i_draw_mask_surface (FrDrawContext* cr,FrDrawSurface* surface,SysDouble surface_x,SysDouble surface_y);
void fr_i_draw_move_to (FrDrawContext* cr,SysDouble x,SysDouble y);
void fr_i_draw_new_path (FrDrawContext* cr);
void fr_i_draw_new_sub_path (FrDrawContext* cr);
void fr_i_draw_paint (FrDrawContext* cr);
void fr_i_draw_paint_with_alpha (FrDrawContext* cr,SysDouble alpha);
void fr_i_draw_path_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_pop_group_to_source (FrDrawContext* cr);
void fr_i_draw_push_group (FrDrawContext* cr);
void fr_i_draw_recording_surface_ink_extents (FrDrawSurface* surface,SysDouble * x0,SysDouble * y0,SysDouble * width,SysDouble * height);
void fr_i_draw_rectangle (FrDrawContext* cr,SysDouble x,SysDouble y,SysDouble width,SysDouble height);
void fr_i_draw_rel_curve_to (FrDrawContext* cr,SysDouble dx1,SysDouble dy1,SysDouble dx2,SysDouble dy2,SysDouble dx3,SysDouble dy3);
void fr_i_draw_rel_line_to (FrDrawContext* cr,SysDouble dx,SysDouble dy);
void fr_i_draw_rel_move_to (FrDrawContext* cr,SysDouble dx,SysDouble dy);
void fr_i_draw_reset_clip (FrDrawContext* cr);
void fr_i_draw_restore (FrDrawContext* cr);
void fr_i_draw_rotate (FrDrawContext* cr,SysDouble angle);
void fr_i_draw_save (FrDrawContext* cr);
void fr_i_draw_scale (FrDrawContext* cr,SysDouble sx,SysDouble sy);
void fr_i_draw_set_dash (FrDrawContext* cr,const SysDouble * dashes,SysInt num_dashes,SysDouble offset);
void fr_i_draw_set_font_size (FrDrawContext* cr,SysDouble size);
void fr_i_draw_set_line_width (FrDrawContext* cr,SysDouble width);
void fr_i_draw_set_source_rgb (FrDrawContext* cr,SysDouble red,SysDouble green,SysDouble blue);
void fr_i_draw_set_source_rgba (FrDrawContext* cr,SysDouble red,SysDouble green,SysDouble blue,SysDouble alpha);
void fr_i_draw_set_source_surface (FrDrawContext* cr,FrDrawSurface* surface,SysDouble x,SysDouble y);
void fr_i_draw_stroke (FrDrawContext* cr);
void fr_i_draw_stroke_extents (FrDrawContext* cr,SysDouble * x1,SysDouble * y1,SysDouble * x2,SysDouble * y2);
void fr_i_draw_stroke_preserve (FrDrawContext* cr);
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
void fr_i_draw_update_layout(FrDrawContext* cr, PangoLayout* layout);
void fr_i_draw_show_layout(FrDrawContext* cr, PangoLayout* layout);

SYS_END_DECLS

#endif
