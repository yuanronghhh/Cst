#include <Framework/Device/FrSdlWindow.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrRender.h>
#include <Framework/Event/FrEventCore.h>

#define SDL_WINDOW_POINTER "FrSdlWindow"
#define USE_SINGLE_WINDOW 1
static void i_window_imp(FrIWindowInterface *iface);
static FrWindowErrFunc err_callback = NULL;

// only single window for performance
static FrSdlWindow *g_window = NULL; 

SYS_DEFINE_TYPE(FrSdlWindow, fr_sdl_window, FR_TYPE_WINDOW);

static FrSdlWindow *gwindow_get_data(SDL_Window *gwindow) {

  return SDL_GetProperty(
    SDL_GetWindowProperties(gwindow),
    SDL_WINDOW_POINTER, NULL);
}

static FrSdlWindow *gwindow_get_data_by_id(SysInt windowID) {
  SDL_Window *gwindow = SDL_GetWindowFromID(windowID);

  return gwindow_get_data(gwindow);
}

static void gwindow_set_data(SDL_Window *gwindow, SysPointer user_data) {
  SDL_SetProperty(
      SDL_GetWindowProperties(gwindow),
      SDL_WINDOW_POINTER, user_data);
}

static FrSdlWindow* sdl_event_get_window(SysInt windowID) {
  FrSdlWindow *window;

#if USE_SINGLE_WINDOW
  window = g_window;
#else
  window = gwindow_get_data_by_id(windowID);
#endif

  return window;
}

static SDL_Window* fr_sdl_window_create_i(
    SysInt width,
    SysInt height,
    const SysChar *title,
    SDL_Window *gshare) {
  SDL_Window *gwindow;
  SysInt flags = 0;

  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

  flags = SDL_WINDOW_VULKAN
    | SDL_WINDOW_RESIZABLE;

  SYS_LEAK_IGNORE_BEGIN;
  gwindow = SDL_CreateWindow(title,
      width,
      height,
      flags);
  SYS_LEAK_IGNORE_END;

  return gwindow;
}

FR_WINDOW_BACKEND_ENUM fr_sdl_window_backend() {

  return FR_WINDOW_GLFW;
}

static void fr_sdl_render_get_framebuffer_size(
    FrRender *render,
    SysInt *width,
    SysInt * height) {
  sys_return_if_fail(render != NULL);

  SDL_GetRenderOutputSize(render->ctx, width, height);
}

static void fr_sdl_window_set_size(FrWindow *window, SysInt width, SysInt height) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_if_fail(self != NULL);

  SDL_SetWindowSize(self->gwindow, width, height);
}

static void fr_sdl_window_set_title(FrWindow *window, const SysChar *title) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_if_fail(self != NULL);

  SDL_SetWindowTitle(self->gwindow, title);
}

static void fr_sdl_window_set_opacity(FrWindow *window, double opacity) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_if_fail(self != NULL);

  SDL_SetWindowOpacity(self->gwindow, (float)opacity);
}

static void fr_sdl_window_set_should_close (FrWindow *window, SysBool bvalue) {
}

static void fr_sdl_window_destroy (FrWindow *window) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);

  SDL_DestroyWindow(self->gwindow);
}

static void fr_sdl_window_get_size(FrWindow *window, SysInt *width, SysInt *height) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_if_fail(self != NULL);

  SDL_GetWindowSize(self->gwindow, width, height);
}

static void fr_sdl_error_callback(void) {
  const SysChar *msg = SDL_GetError();

  if(err_callback) {

    err_callback(-1, msg);
  }
  SDL_ClearError();
}

static void fr_sdl_set_error_callback (FrWindowErrFunc callback) {
  sys_return_if_fail(callback != NULL);

  err_callback = callback;
}

SysInt fr_sdl_get_key(FrWindow *window, SysInt key) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_val_if_fail(self != NULL, -1);

  return SDL_GetModState();
}

const SysChar* fr_sdl_get_key_name(SysInt unused, SysInt scancode) {

  return SDL_GetScancodeName(scancode);
}

void fr_sdl_window_set_gwindow(FrWindow *window, SDL_Window * gwindow) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_if_fail(self != NULL);

  self->gwindow = gwindow;
}

SDL_Window * fr_sdl_window_get_gwindow(FrWindow *window) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_val_if_fail(self != NULL, NULL);

  return self->gwindow;
}

/* event callbacks */
static void fr_sdl_window_key_callback(FrSdlWindow* gwindow,
    SysInt key,
    SysInt scancode,
    SysInt action,
    SysInt mods) {
  FrWindow *window = FR_WINDOW(gwindow);

  FrEvent *e = fr_event_key_new_I(window, key, scancode, action, mods);

  fr_events_push_head(e);
}

static void fr_sdl_window_mouse_button_callback(FrSdlWindow* gwindow,
    SysInt button, 
    SysInt action, 
    SysInt mods) {
  FrWindow *window = FR_WINDOW(gwindow);

  FrEvent *e = fr_event_mousekey_new_I(window, button, action, mods);

  fr_events_push_head(e);
}

static void fr_sdl_window_cursor_pos_callback(FrSdlWindow* gwindow,
    SysDouble xpos, 
    SysDouble ypos) {
  FrWindow *window = FR_WINDOW(gwindow);

  FrEvent *e = fr_event_cursor_move_new_I(window, xpos, ypos);

  fr_events_push_head(e);
}

static void fr_sdl_window_close_callback(FrSdlWindow* gwindow) {
  FrWindow *window = FR_WINDOW(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_WINDOW_CLOSE);

  fr_events_push_head(e);
}

static void fr_sdl_window_scroll_callback(FrSdlWindow* gwindow,
    SysDouble xoffset, 
    SysDouble yoffset) {

  sys_debug_N("%s", "window_scroll");
}

static void fr_sdl_window_cursor_enter_callback(FrSdlWindow* gwindow,
    SysInt entered) {
  // sys_debug_N("%s", "cursor_enter");
}

static void fr_sdl_window_maximize_callback(SDL_Window* window, int maximized) {
}

static void fr_sdl_window_framebuffer_size_callback(FrSdlWindow* gwindow,
    SysInt width, 
    SysInt height) {
  FrWindow *window = FR_WINDOW(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_FRAMEBUFFER_RESIZE);

  fr_events_push_head(e);
}

static void fr_sdl_window_focus_callback(FrSdlWindow* gwindow, SysInt focused) {
  sys_debug_N("window_focus :%d", focused);
}

static void fr_sdl_window_size_callback(FrSdlWindow* gwindow,
    SysInt width, 
    SysInt height) {
  FrWindow *window = FR_WINDOW(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_WINDOW_RESIZE);

  fr_events_push_head(e);
}

static void fr_sdl_window_pos_callback(FrSdlWindow* gwindow,
    SysInt xpos, 
    SysInt ypos) {
  // sys_debug_N("%s", "window_pos");
}

static void fr_sdl_window_refresh_callback(FrSdlWindow* gwindow) {
  FrWindow *window = FR_WINDOW(gwindow);

  FrEvent *e = fr_event_refresh_new_I(window);

  fr_events_dispatch(e);
}

static void sdl_handle_event(SDL_Event *e) {
  FrSdlWindow *window;

  switch(e->type) {
    case SDL_EVENT_WINDOW_SHOWN:
      break;
    case SDL_EVENT_WINDOW_EXPOSED:
      window = sdl_event_get_window(e->window.windowID);
      fr_sdl_window_refresh_callback(window);
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      window = sdl_event_get_window(e->window.windowID);
      fr_sdl_window_size_callback(window, -1, -1);
      break;
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      window = sdl_event_get_window(e->window.windowID);
      fr_sdl_window_framebuffer_size_callback(window, -1, -1);
      break;
    case SDL_EVENT_WINDOW_DESTROYED:
      window = sdl_event_get_window(e->window.windowID);
      fr_sdl_window_close_callback(window);
      break;
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      window = sdl_event_get_window(e->window.windowID);
      fr_sdl_window_close_callback(window);
      break;
    case SDL_EVENT_WINDOW_MOVED:
      break;
    case SDL_EVENT_KEY_DOWN:
      window = sdl_event_get_window(e->window.windowID);
      fr_sdl_window_close_callback(window);
      break;
    case SDL_EVENT_KEY_UP:
      window = sdl_event_get_window(e->window.windowID);
      SDL_KeyboardEvent ke = e->key;
      fr_sdl_window_key_callback(window, ke.key, ke.scancode, ke.state, ke.mod);
      break;
    case SDL_EVENT_TEXT_EDITING:
      break;
    case SDL_EVENT_TEXT_INPUT:
      break;
    case SDL_EVENT_KEYMAP_CHANGED:
      break;
    case SDL_EVENT_QUIT:
      window = sdl_event_get_window(e->window.windowID);
      fr_sdl_window_close_callback(window);
      break;
  }
}

/* window setup */
static void fr_delay_i(SysUInt msec) {
  SDL_Delay((SysUInt)msec);
}

static void fr_wait_events_timeout_i(SysInt msec) {
  SDL_Event e;

  if (SDL_WaitEventTimeout(&e, msec)) {
  }

  sdl_handle_event(&e);
}

static void fr_wait_events_i(void) {
  SDL_Event e;
  SDL_WaitEvent(&e);
  sdl_handle_event(&e);
}

static void fr_post_empty_event_i(void) {
  SDL_Event e;

  /* sdl will copy memory */
  SDL_PushEvent(&e);
}

static void fr_poll_events_i(void) {
  SDL_Event e;
  SDL_PollEvent(&e);

  sdl_handle_event(&e);
}

static void fr_swap_buffers_i(FrWindow *o) {
  FrSdlWindow *self = FR_SDL_WINDOW(o);

  sys_return_if_fail(self != NULL);
  SDL_GL_SwapWindow(self->gwindow);
}

void fr_window_display_create (FrDisplay *display) {
#if SYS_OS_WIN32
  display->native_ctx = UINT_TO_POINTER(SDL_GetPrimaryDisplay());
#elif SYS_OS_UNIX
  display->native_ctx = (SysPointer)XOpenDisplay(NULL);
#endif
}

static SysPointer fr_window_get_native_display (FrDisplay *display) {

  return display->native_ctx;
}

static SysPointer fr_window_get_native_window (FrWindow* window) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);

  return self->native_ctx;
}

void fr_sdl_window_setup(void) {
  SYS_LEAK_IGNORE_BEGIN;
  if (SDL_Init(
        SDL_INIT_VIDEO
        | SDL_INIT_AUDIO
        | SDL_INIT_TIMER) < 0) {
    sys_error_N("SDL failed to init: %s", SDL_GetError());
  }
  SYS_LEAK_IGNORE_END;
}

void fr_sdl_window_teardown(void) {

  SDL_Quit();
}

#if defined(VK_VERSION_1_0)
void fr_sdl_window_create_vk_surface(FrWindow *window,
    VkInstance instance,
    VkSurfaceKHR *surfacekhr) {
  if (SDL_CreateWindowSurface(instance,
        self->gwindow,
        NULL,
        surfacekhr) != VK_SUCCESS) {

    sys_error_N("%s", SYS_("failed to create vulkan surface"));
  }
}
#endif

static void fr_sdl_window_create(
    FrWindow *o,
    SysInt width,
    SysInt height,
    const SysChar *title,
    FrWindow *share) {

  FrSdlWindow *self = FR_SDL_WINDOW(o);
  FrSdlWindow *sshare = FR_SDL_WINDOW(share);
  SDL_Window *gwindow = NULL;
  SDL_Window *gshare = share == NULL ? NULL : sshare->gwindow;

  gwindow = fr_sdl_window_create_i(800, 600, title, gshare);
  self->gwindow = gwindow;

#if SYS_OS_WIN32
  HWND hwnd = (HWND)SDL_GetProperty(
    SDL_GetWindowProperties(gwindow),
    SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

  self->native_ctx = hwnd;
#elif SYS_OS_UNIX

  self->native_ctx = UINT_TO_POINTER(SDL_GetNumberProperty(
      SDL_GetWindowProperties(gwindow),
      SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
#endif

  if(self->native_ctx == NULL) {

    sys_error_N("failed to get native window handle: %s", SDL_GetError());
  }

  gwindow_set_data(gwindow, self);

#if USE_SINGLE_WINDOW
  g_window = self;
#endif
}

static void fr_window_destroy(FrWindow *o) {
  FrSdlWindow *self = FR_SDL_WINDOW(o);

  SDL_DestroyWindow(self->gwindow);
}

static void i_window_imp(FrIWindowInterface *iface) {
  iface->create = fr_sdl_window_create;
  iface->set_error_callback = fr_sdl_set_error_callback;
  iface->get_framebuffer_size = fr_sdl_render_get_framebuffer_size;
  iface->window_get_size = fr_sdl_window_get_size;
  iface->window_set_size = fr_sdl_window_set_size;
  iface->window_set_title = fr_sdl_window_set_title;
  iface->window_set_opacity = fr_sdl_window_set_opacity;
  iface->window_set_should_close = fr_sdl_window_set_should_close;
  iface->window_destroy = fr_window_destroy;
  iface->swap_buffers = fr_swap_buffers_i;
  iface->get_key_name = fr_sdl_get_key_name;
  iface->get_key = fr_sdl_get_key;
  iface->wait_events_timeout = fr_wait_events_timeout_i;
  iface->wait_events = fr_wait_events_i;
  iface->post_empty_event = fr_post_empty_event_i;
  iface->poll_events = fr_poll_events_i;
  iface->delay = fr_delay_i;
  iface->get_native_window = fr_window_get_native_window;
  iface->get_native_display = fr_window_get_native_display;
  iface->display_create = fr_window_display_create;
}

void fr_sdl_window_iface_setup(FrIWindowInterface *iface) {

  i_window_imp(iface);
}

FrWindow* fr_sdl_window_new(void) {

  return sys_object_new(FR_TYPE_SDL_WINDOW, NULL);
}

static void fr_sdl_window_construct(
    FrWindow *o,
    FrWindowContext *info) {

  FR_WINDOW_CLASS(fr_sdl_window_parent_class)->construct(o, info);
}

static void fr_sdl_window_dispose(SysObject* o) {
  FrSdlWindow *self = FR_SDL_WINDOW(o);

  fr_window_destroy(FR_WINDOW(self));

  SYS_OBJECT_CLASS(fr_sdl_window_parent_class)->dispose(o);
}

static void fr_sdl_window_class_init(FrSdlWindowClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrWindowClass *wcls = FR_WINDOW_CLASS(cls);

  wcls->construct = fr_sdl_window_construct;
  ocls->dispose = fr_sdl_window_dispose;
}

void fr_sdl_window_init(FrSdlWindow* self) {
}
