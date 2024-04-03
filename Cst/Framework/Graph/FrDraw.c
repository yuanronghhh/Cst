#include <Framework/Graph/FrDraw.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrContext.h>
#include <Framework/Graph/FrCairoDraw.h>
#include <Framework/Graph/FrSurface.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrIDevice.h>

static FrDraw *g_draw = NULL;

SYS_DEFINE_TYPE(FrDraw, fr_draw, SYS_TYPE_OBJECT);

void fr_draw_setup(const SysChar *name) {
  sys_assert(g_draw == NULL);
  fr_font_setup();

  if(sys_str_equal(name, "cairo")) {
    g_draw = fr_cairo_draw_new_I();
  }
}

void fr_draw_teardown(void) {
  sys_assert(g_draw != NULL);
  fr_font_teardown();

  sys_clear_pointer(&g_draw, _sys_object_unref);
}

FrIDraw * fr_draw_get_g_idraw(void) {
  sys_assert(g_draw != NULL && "FrIDrawInterface must be inited before use.");

  return FR_I_DRAW(g_draw);
}

FrIDrawInterface* fr_draw_get_iface(void) {
  sys_assert(g_draw != NULL && "FrIDrawInterface must be inited before use.");

  FrIDrawInterface *iface = FR_I_DRAW_GET_IFACE(g_draw);

  return iface;
}

static void fr_draw_construct(FrDraw *self) {
}

/* object api */
FrDraw* fr_draw_new(void) {
  return sys_object_new(FR_TYPE_DRAW, NULL);
}

static void fr_draw_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_draw_parent_class)->dispose(o);
}

static void fr_draw_class_init(FrDrawClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_draw_construct;
  ocls->dispose = fr_draw_dispose;
}

void fr_draw_init(FrDraw *self) {
}
