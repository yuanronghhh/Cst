#include <FrameworkTest/tests/TestVideo.h>

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

static FrRegion* region_create(FrDevice *device) {
  FrBound bound = {0};

  fr_i_device_get_size(FR_I_DEVICE(device), &bound.width, &bound.height);
  return fr_region_create_rectangle(&bound);
}

static void render_render(FrDrawContext *draw_context, FrDevice *device) {
  sys_return_if_fail(draw_context != NULL);
  FrRegion* region = region_create(device);

  fr_draw_context_frame_begin(draw_context, region);

  fr_draw_context_frame_end(draw_context, region);

  sys_clear_pointer(&region, fr_region_destroy);
}

void test_fr_draw_context(void) {
  FrWindow *window;
  FrDisplay* display;
  FrDevice *device;
  FrDrawContext *draw_context;
  SysHArray *surfaces;
  FrSurface* paint_surface;

  display = fr_display_new_I();
  window = fr_window_top_new(display);
  device = FR_DEVICE(window);

  draw_context = fr_cairo_draw_context_new_I(device);
  fr_draw_context_g_set(draw_context);

  surfaces = sys_harray_new_with_free_func((SysDestroyFunc)_sys_object_unref);

  FrSurfaceContext info = {.width = 800, .height= 600};
  paint_surface = fr_surface_new_I(&info);

  fr_draw_context_add_surface(draw_context, paint_surface);

  render_render(draw_context, device);

  sys_clear_pointer(&device, _sys_object_unref);
  sys_clear_pointer(&display, _sys_object_unref);
  sys_harray_free(surfaces, true);
  fr_draw_context_g_unset();
}

void render_callback(FrMediaPlayer *player) {

}

void test_video_player(void) {
  FrWindow *window;
  FrDisplay* display;
  FrDevice *device;
  FrDrawContext *draw_context;
  FrMediaFile *mfile;
  FrMediaPlayer *mplayer;
  FrIMediaRender *imrender;
  FrSurface* paint_surface;

  display = fr_display_new_I();
  window = fr_window_top_new(display);
  device = FR_DEVICE(window);

  draw_context = fr_cairo_draw_context_new_I(device);
  fr_draw_context_g_set(draw_context);

  FrSurfaceContext info = {.width = 800, .height= 600};
  paint_surface = fr_surface_new_I(&info);

  fr_draw_context_add_surface(draw_context, paint_surface);

  imrender = FR_I_MEDIA_RENDER(draw_context);
  mfile = fr_media_file_new_I(TEST_VIDEO_FILE);
  mplayer = fr_media_player_new_I(mfile);

  fr_media_player_set_render(mplayer, imrender);
  fr_media_player_run(mplayer);

  sys_object_unref(mplayer);
  sys_object_unref(mfile);
  sys_object_unref(draw_context);

  sys_clear_pointer(&device, _sys_object_unref);
  sys_clear_pointer(&display, _sys_object_unref);

  fr_draw_context_g_unset();
}

static void test_avformat_leak(void) {
  FrMediaFile *mfile;

  mfile = fr_media_file_new_I(TEST_VIDEO_FILE);

  sys_clear_pointer(&mfile, _sys_object_unref);
}

static void test_scale_image(void) {
  FrImageScaleContext scale_info = {
    .in_width = 800,
    .in_height = 600,
    .out_width = 800,
    .out_height = 600,
    .in_pix_fmt = AV_PIX_FMT_BGRA,
    .out_pix_fmt = AV_PIX_FMT_RGBA
  };

  FrSurface *sur;
  FrImage *src;
  FrImage *dst;

  FrDrawContext *draw;
  FrImageScale *scale;
  FrImageSaver *saver;
  FrContext *cr;
  SysInt *stride;

  FrSurfaceContext sur_args = { .width = 800, .height = 600 };
  SysChar* filename = FR_PROJECT_DIR"/Assets/surface.png";

  draw = fr_cairo_draw_context_new_I(NULL);
  fr_i_draw_setup(FR_I_DRAW(draw));

  sur = fr_surface_new_I(&sur_args);
  cr = fr_context_new_I(sur);
  fr_context_stoke_debug(cr, 0);
  fr_i_draw_surface_flush(sur);
  sys_object_unref(cr);

  scale = fr_image_scale_new_I(&scale_info);
  saver = fr_image_saver_new_I();

  fr_i_draw_surface_save_to_png(sur, filename);

  src = fr_image_new_from_surface(sur);
  stride = fr_image_get_stride(src);

  FrImageContext image_info = {
    .width = 800,
    .height = 300,
    .format = scale_info.out_pix_fmt,
  };
  image_info.stride[0] = stride[0];

  dst = fr_image_new_with_buffer(&image_info);

  for (int i = 0 ; i < 3; i++) {

    fr_image_scale_convert_image(scale, src, dst);
    fr_image_saver_save_png(saver, dst, filename);
  }

  sys_object_unref(sur);
  sys_object_unref(scale);
  sys_object_unref(src);
  sys_object_unref(dst);
  sys_object_unref(saver);
  fr_i_draw_teardown();
  sys_object_unref(draw);
}


void test_fr_window_leak(void) {
  GLFWwindow *gwindow;

  gwindow = glfwCreateWindow(800, 600, "Leak Demo", NULL, NULL);

  glfwDestroyWindow(gwindow);
  glfwTerminate();
}

void test_video_init(int argc, SysChar * argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_scale_image);
    // RUN_TEST(test_avformat_leak);
    // RUN_TEST(test_video_player);
    // RUN_TEST(test_fr_draw_context);
    // RUN_TEST(test_fr_basic);
    // RUN_TEST(test_fr_window_leak);
    // RUN_TEST(test_fr_window_basic);
  }
  UNITY_END();
}
