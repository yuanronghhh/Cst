#ifndef __FR_FONT_H__
#define __FR_FONT_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <fontconfig/fontconfig.h>
#include <freetype/freetype.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <pango/pangofc-fontmap.h>


void fr_font_setup(void);
void fr_font_teardown(void);


SYS_END_DECLS

#endif

