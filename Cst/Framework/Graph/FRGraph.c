#include <Framework/Graph/FRGraph.h>

static SysInt fr_color_parse_hex(const SysChar *p, SysInt length) {
  SysInt i = 0;
  SysInt len = 0;

  while (len < length) {
    i <<= 4;
    if (p[len] >= 'a' && p[len] <= 'f') {
      i += ((p[len] - 'a') + 10);

    } else if (p[len] >= 'A' && p[len] <= 'F') {
      i += ((p[len] - 'A') + 10);
    } else {
      i += (p[len] - '0');
    }

    len++;
  }

  return i;
}

SysBool fr_color_rgba_parse(const SysChar *rgba, FRColor *color) {
  sys_return_val_if_fail(rgba != NULL, false);

  const SysChar *c = rgba;

  if (*c == '#') {
    c++;
  }

  color->r = fr_color_parse_hex(c, 2) / 255.0;
  color->g = fr_color_parse_hex(c + 2, 2) / 255.0;
  color->b = fr_color_parse_hex(c + 4, 2) / 255.0;
  color->a = 1.0;

  if (*(c + 6) != '\0') {
    color->a = fr_color_parse_hex(c + 6, 2) / 255.0;
  }

  sys_assert(color->r <= 1.0 && "color red parse failed");
  sys_assert(color->g <= 1.0 && "color green parse failed");
  sys_assert(color->b <= 1.0 && "color blue parse failed");
  sys_assert(color->a <= 1.0 && "color alpha parse failed");

  return true;
}

FRColor *fr_color_rgba_new(const SysChar *rgba) {
  FRColor *color = sys_new_N(FRColor, 1);
  fr_color_rgba_parse(rgba, color);
  return color;
}

FRColor *fr_color_clone(FRColor *oself) {
  FRColor *nself = sys_new_N(FRColor, 1);

  nself->r = oself->r;
  nself->g = oself->g;
  nself->b = oself->b;
  nself->a = oself->a;

  return nself;
}

SysBool fr_rect_in_range(const FRRect *rect, SysInt x, SysInt y) {
  return (x >= rect->x && y >= rect->y && x <= rect->x + rect->width && y <= rect->y + rect->height);
}

FRSInt4* fr_sint4_clone(FRSInt4 *m) {
  FRSInt4* o = fr_sint4_new();

  o->m0 = m->m0;
  o->m1 = m->m1;
  o->m2 = m->m2;
  o->m3 = m->m3;

  return o;
}

FRSInt4* fr_sint4_new(void) {
  FRSInt4* o = sys_new_N(FRSInt4, 1);

  return o;
}

void fr_sint4_free(FRSInt4* o) {
  sys_free_N(o);
}
