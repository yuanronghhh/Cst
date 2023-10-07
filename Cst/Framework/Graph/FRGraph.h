#ifndef __FR_GRAPH_H__
#define __FR_GRAPH_H__

#include <Framework/ThirdParty/FRCairo.h>
#include <pixman.h>

SYS_BEGIN_DECLS

typedef  struct  _FRRGBA           FRColor;
typedef  struct  _FRPoint          FRPoint;
typedef  struct  _FRInt4           FRInt4;
typedef  struct  _FRSInt4          FRSInt4;
typedef  struct _cairo_region  FRRegion;
typedef  struct _cairo_rectangle_int FRRect;

typedef  struct _cairo_pattern FRPattern;
typedef  struct _cairo_surface FRSurface;
typedef  struct _cairo FRContext;

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
SYS_API SysBool fr_rect_in_range(const FRRect *rect, SysInt x, SysInt y);
FRSInt4* fr_sint4_clone(FRSInt4* m);
FRSInt4* fr_sint4_new(void);
void fr_sint4_free(FRSInt4* o);
#define fr_region_contains_rectangle cairo_region_contains_rectangle
#define fr_region_intersect_rectangle cairo_region_intersect_rectangle

#define fr_region_is_empty cairo_region_is_empty

SYS_END_DECLS

#endif
