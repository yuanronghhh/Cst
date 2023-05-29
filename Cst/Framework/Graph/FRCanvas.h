#ifndef __FR_CANVAS_H__
#define __FR_CANVAS_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define fr_region_destroy cairo_region_destroy
#define fr_region_create_rectangle cairo_region_create_rectangle
#define fr_region_create cairo_region_create
#define fr_region_union_rectangle cairo_region_union_rectangle

#define fr_context_destroy cairo_destroy
#define fr_surface_destroy cairo_surface_destroy
#define fr_surface_image_surface_create cairo_image_surface_create

#define fr_pattern_destroy cairo_destroy_pattern
#define fr_pattern_create_for_surface cairo_pattern_create_for_surface

#define fr_context_pop_group_to_source cairo_pop_group_to_source
#define fr_context_set_source cairo_set_source
#define fr_context_mask cairo_mask
#define fr_context_push_group cairo_push_group
#define fr_context_pop_group cairo_pop_group
#define fr_context_save cairo_save
#define fr_context_restore cairo_restore
#define fr_context_set_font_size cairo_set_font_size
#define fr_context_rectangle cairo_rectangle
#define fr_context_set_line_width cairo_set_line_width
#define fr_context_show_text_glyphs cairo_show_text_glyphs
#define fr_context_rel_line_to cairo_rel_line_to
#define fr_context_close_path cairo_close_path
#define fr_context_stroke cairo_stroke
#define fr_context_stroke_preserve cairo_stroke_preserve
#define fr_context_move_to cairo_move_to
#define fr_context_set_source_rgba cairo_set_source_rgba
#define fr_context_surface_flush cairo_surface_flush
#define fr_context_get_target cairo_get_target
#define fr_context_fill cairo_fill
#define fr_context_paint cairo_paint
#define fr_context_clear cairo_clear
#define fr_context_arc cairo_arc
#define fr_context_new_path cairo_new_path
#define fr_context_set_font_face cairo_set_font_face
#define fr_context_get_scaled_font cairo_get_scaled_font
#define fr_context_scaled_font_get_font_face cairo_scaled_font_get_font_face

#define fr_context_get_font_face cairo_get_font_face
#define fr_context_font_face_destroy cairo_font_face_destroy
#define fr_context_select_font_face cairo_select_font_face
#define fr_context_font_extents cairo_font_extents
#define fr_context_text_extents cairo_text_extents
#define fr_context_get_font_options cairo_get_font_options
#define fr_context_font_options_destroy cairo_font_options_destroy
#define fr_text_cluster_allocate cairo_text_cluster_allocate
#define fr_text_cluster_free cairo_text_cluster_free

#define FRTextClusterFlags cairo_text_cluster_flags_t
#define FRTextCluster cairo_text_cluster_t
#define FRGlyph cairo_glyph_t
#define FRTextExtents cairo_text_extents_t
#define FRFontFace cairo_font_face_t
#define FRSCaledFont cairo_scaled_font_t

void fr_canvas_setup(void);
void fr_canvas_teardown(void);

SYS_API void fr_draw_frame_begin(FRDraw *self, FRRegion *region);
SYS_API void fr_draw_frame_end(FRDraw *self, FRRegion *region);

SYS_API FRContext* fr_draw_create_context(FRDraw* draw);
SYS_API FRContext * fr_draw_get_cr(FRDraw * draw);
SYS_API FRDraw *fr_canvas_create_draw (FRWindow *window);
SYS_API void fr_draw_destroy (FRDraw* draw);
SYS_API FRSurface *fr_draw_get_surface (FRDraw *draw);
SYS_API FRSurface* fr_draw_create_surface (FRDraw* draw);
SYS_API SysInt fr_canvas_rounded_rectangle(FRContext * context, SysDouble x, SysDouble y, SysDouble w, SysDouble h, SysDouble radius);

SYS_END_DECLS

#endif

