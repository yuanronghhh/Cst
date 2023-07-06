#ifndef __FR_GRAPH_H__
#define __FR_GRAPH_H__

#include <Framework/FRCommon.h>
#include <pixman.h>

SYS_BEGIN_DECLS

typedef  struct  _FRRGBA           FRColor;
typedef  struct  _FRPoint          FRPoint;
typedef  struct  _FRInt4           FRInt4;
typedef  struct  _FRSInt4          FRSInt4;

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

struct _FRInt4 {
  SysInt m0;
  SysInt m1;
  SysInt m2;
  SysInt m3;
};

struct _FRSInt4 {
  SysInt16 m0;
  SysInt16 m1;
  SysInt16 m2;
  SysInt16 m3;
};

#define FRImage pixman_image_t

SYS_API SysBool fr_color_rgba_parse(const SysChar *rgba, FRColor *color);
SYS_API FRColor* fr_color_rgba_new(const SysChar *rgba);
SYS_API SysBool fr_rect_in_range(const FRRect *rect, SysInt x, SysInt y) ;
#define fr_region_contains_rectangle cairo_region_contains_rectangle
#define fr_region_intersect_rectangle cairo_region_intersect_rectangle

#define fr_region_is_empty cairo_region_is_empty

SYS_END_DECLS

#endif
