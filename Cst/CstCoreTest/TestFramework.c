#include "TestFramework.h"
#include <CstCore/Driver/CstSurface.h>

static const SysChar *FR_ACTION_NAMES[] = {
  "key_down",
  "key_up",
  "drag_start",
  "drag_end",
};

void test_fr_basic(void) {
  SysInt ftype = fr_get_type_by_name((const SysChar **)FR_ACTION_NAMES, ARRAY_SIZE(FR_ACTION_NAMES), "key_up");

  UNUSED(ftype);
}

void test_fr_window_basic(void) {
  FrDisplay *display = fr_display_new_I();
  FrWindow *window = fr_window_top_new(display);

  sys_object_unref(window);
  sys_object_unref(display);
}

static FrRegion* region_create(FrIDevice *idevice) {
  FrBound bound = {0};
  fr_i_device_get_size(idevice, &bound.width, &bound.height);
  return fr_region_create_rectangle(&bound);
}

static void render_render(FrDrawContext *draw_context, FrIDevice *idevice) {
  sys_return_if_fail(draw_context != NULL);
  FrRegion* region = region_create(idevice);

  fr_draw_context_frame_begin(draw_context, region);

  fr_draw_context_frame_end(draw_context, region);

  sys_clear_pointer(&region, fr_region_destroy);
}

void test_fr_draw_context(void) {
  FrWindow *window;
  FrDisplay* display;
  FrIDevice *idevice;
  FrIDraw* idraw;
  FrDrawContext *draw_context;
  SysHArray *surfaces;
  CstSurface* paint_surface;

  display = fr_display_new_I();
  window = fr_window_top_new(display);
  idevice = FR_I_DEVICE(window);
  idraw = fr_draw_get_g_idraw();

  surfaces = sys_harray_new_with_free_func((SysDestroyFunc)_sys_object_unref);
  paint_surface = cst_surface_create_image_surface(800, 600);

  draw_context = fr_draw_context_new_I(idraw, idevice);
  fr_draw_context_add_surface(draw_context, FR_SURFACE(paint_surface));

  render_render(draw_context, idevice);
  render_render(draw_context, idevice);

  sys_clear_pointer(&draw_context, _sys_object_unref);
  sys_clear_pointer(&idevice, _sys_object_unref);
  sys_clear_pointer(&display, _sys_object_unref);
  sys_harray_free(surfaces, true);
}

void test_fr_window_leak(void) {
  GLFWwindow *gwindow;

  gwindow = glfwCreateWindow(800, 600, "Leak Demo", NULL, NULL);

  glfwDestroyWindow(gwindow);
  glfwTerminate();
}

void test_fr_init(int argc, SysChar * argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_fr_draw_context);
    // RUN_TEST(test_fr_basic);
    // RUN_TEST(test_fr_window_leak);
    // RUN_TEST(test_fr_window_basic);
  }
  UNITY_END();
}
