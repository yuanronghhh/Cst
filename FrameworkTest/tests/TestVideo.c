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
  FrWindow *window = fr_window_new_I(display, NULL);

  sys_object_unref(window);
  sys_object_unref(display);
}

void test_fr_sdl_basic(void) {
  SDL_Window *gwindow;

  gwindow = SDL_CreateWindow("abc", 800, 600, SDL_WINDOWPOS_CENTERED);

  SDL_DestroyWindow(gwindow);
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
  surfaces = sys_harray_new_with_free_func((SysDestroyFunc)_sys_object_unref);

  FrSurfaceContext info = {.width = 800, .height= 600};
  paint_surface = fr_surface_new_I(&info);

  fr_draw_context_add_surface(draw_context, paint_surface);

  render_render(draw_context, device);

  sys_clear_pointer(&device, _sys_object_unref);
  sys_clear_pointer(&display, _sys_object_unref);
  sys_harray_free(surfaces, true);
}

void render_callback(FrAvPlayer *player) {

}

void test_video_player(void) {
  FrWindow *window;
  FrDisplay* display;
  FrDevice *video_device;
  FrAudioDevice *audio_device;
  FrMediaFile *mfile;
  FrPlayer *mplayer;
  FrAvRender *imrender;
  FrDrawContext *video_render = NULL;
  FrAudioStream *audio_render = NULL;
  FrSurface* paint_surface;

  display = fr_display_new_I();
  window = fr_window_top_new(display);
  video_device = FR_DEVICE(window);

  mfile = fr_media_file_new_I(TEST_VIDEO_FILE);
  sys_return_if_fail(mfile != NULL);

  audio_device = (FrAudioDevice *)fr_audio_device_find_by_media_file(mfile);
  if(audio_device) {

    audio_render = (FrAudioStream *)fr_audio_stream_new_out_by_device(audio_device);
  }

  video_render = fr_cairo_draw_context_new_I(video_device);

  FrAvRenderContext mrinfo = {
    .video_render = video_render,
    .audio_render = audio_render
  };
  imrender = fr_av_render_new_I(&mrinfo);

  FrSurfaceContext sinfo = {
    .width = 800,
    .height= 600};
  paint_surface = fr_surface_new_I(&sinfo);

  fr_draw_context_add_surface(video_render, paint_surface);

  FrAvPlayerContext pinfo = {
    .file = mfile,
    .window = window,
    .render = imrender};
  mplayer = fr_av_player_new_I(&pinfo);

  fr_player_run(mplayer);

  sys_clear_pointer(&imrender, _sys_object_unref);
  sys_clear_pointer(&mplayer, _sys_object_unref);
  sys_clear_pointer(&mfile, _sys_object_unref);

  if(video_render != NULL) {

    sys_clear_pointer(&video_render, _sys_object_unref);
  }

  if(audio_render != NULL) {

    sys_clear_pointer(&audio_render, _sys_object_unref);
  }

  sys_clear_pointer(&video_device, _sys_object_unref);
  sys_clear_pointer(&display, _sys_object_unref);
}

static void test_avformat_leak(void) {
  FrMediaFile *mfile;

  mfile = fr_media_file_new_I(TEST_VIDEO_FILE);

  AVPacket *ctx = av_packet_alloc();
  AVPacket *ctx2 = av_packet_alloc();

  av_packet_ref(ctx2, ctx);

  av_packet_free(&ctx2);
  av_packet_free(&ctx);

  sys_clear_pointer(&mfile, _sys_object_unref);
}

void test_fr_window_leak(void) {
  FrDisplay *display = fr_display_new_I();
  FrWindow *window = fr_window_top_new(display);

  sys_object_unref(window);
}

void test_video_init(int argc, const SysChar * argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_video_player);
    // RUN_TEST(test_fr_sdl_basic);
    // RUN_TEST(test_avformat_leak);
    // RUN_TEST(test_fr_draw_context);
    // RUN_TEST(test_fr_basic);
    // RUN_TEST(test_fr_window_leak);
    // RUN_TEST(test_fr_window_basic);
  }
  UNITY_END();
}
