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

SysBool fr_rect_in_range(const FRRect *rect, SysInt x, SysInt y) {
  return (x >= rect->x && y >= rect->y && x <= rect->x + rect->width && y <= rect->y + rect->height);
}