#ifndef __FR_GRAPH_H__
#define __FR_GRAPH_H__

#include <Framework/FRCommon.h>
#include <pixman.h>

SYS_BEGIN_DECLS

typedef  struct  _FRRGBA           FRColor;
typedef  struct  _FRPoint          FRPoint;

struct _FRPoint {
  SysInt x;
  SysInt y;
};

struct _FRRGBA {
  SysDouble r;
  SysDouble g;
  SysDouble b;
  SysDouble a;
};

#define FRImage pixman_image_t

SYS_API SysBool fr_color_rgba_parse(const SysChar *rgba, FRColor *color);
SYS_API FRColor* fr_color_rgba_new(const SysChar *rgba);
SYS_API SysBool fr_rect_in_range(const FRRect *rect, SysInt x, SysInt y) ;
SYS_API SysBool fr_rect_is_overlap(const FRRect * rc1, const FRRect * rc2);

SYS_END_DECLS

#endif
