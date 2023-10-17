#ifndef __FR_FONT_H__
#define __FR_FONT_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <fontconfig/fontconfig.h>
#include <freetype/freetype.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <pango/pangofc-fontmap.h>

typedef  struct  _PangoLayout FRDrawLayout;

#define fr_layout_update_layout pango_cairo_update_layout
#define fr_layout_get_pixel_size pang_layout_get_pixel_size
#define fr_layout_set_font_description pango_layout_set_font_description

void fr_font_setup(void);
void fr_font_teardown(void);


SYS_END_DECLS

#endif

