#include <Framework/Device/FrSdlWindow.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Event/FrEventCore.h>

#define fr_sdl_get_window(gwindow) ((FrWindow *)SDL_GetWindowData(gwindow, "sdl_user_data"))
#define fr_sdl_set_window(gwindow, window) SDL_SetWindowData(gwindow, "sdl_user_data", window)

static void fr_sdl_window_event_register(FrSdlWindow *self);
static void fr_sdl_window_event_unregister(FrSdlWindow *self);
static void i_window_imp(FrIWindowInterface *iface);
static FrWindowErrFunc err_callback = NULL;

SYS_DEFINE_TYPE(FrSdlWindow, fr_sdl_window, FR_TYPE_WINDOW);

static SDL_Window* fr_sdl_window_create_i(
    SysInt width,
    SysInt height,
    const SysChar *title,
    SDL_Window *gshare) {
  SDL_Window *gwindow;

  SYS_LEAK_IGNORE_BEGIN;
  gwindow = SDL_CreateWindow(title,
      SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 
      width, 
      height, 
      0);
  SYS_LEAK_IGNORE_END;

  return gwindow;
}

FR_WINDOW_BACKEND_ENUM fr_sdl_window_backend() {

  return FR_WINDOW_GLFW;
}

static void fr_sdl_window_get_framebuffer_size(
    FrWindow *window,
    SysInt *width, 
    SysInt * height) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  sys_return_if_fail(window != NULL);

  SDL_GL_GetDrawableSize(self->gwindow, width, height);
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
static void fr_sdl_window_key_callback(SDL_Window* gwindow,
    SysInt key, 
    SysInt scancode, 
    SysInt action, 
    SysInt mods) {
  FrWindow *window = fr_sdl_get_window(gwindow);

  FrEvent *e = fr_event_key_new_I(window, key, scancode, action, mods);

  fr_events_push_head(e);
}

static void fr_sdl_window_mouse_button_callback(SDL_Window* gwindow,
    SysInt button, 
    SysInt action, 
    SysInt mods) {
  FrWindow *window = fr_sdl_get_window(gwindow);

  FrEvent *e = fr_event_mousekey_new_I(window, button, action, mods);

  fr_events_push_head(e);
}

static void fr_sdl_window_cursor_pos_callback(SDL_Window* gwindow,
    SysDouble xpos, 
    SysDouble ypos) {
  FrWindow *window = fr_sdl_get_window(gwindow);

  FrEvent *e = fr_event_cursor_move_new_I(window, xpos, ypos);

  fr_events_push_head(e);
}

static void fr_sdl_window_close_callback(SDL_Window* gwindow) {
  FrWindow *window = fr_sdl_get_window(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_WINDOW_CLOSE);

  fr_events_push_head(e);
}

static void fr_sdl_window_scroll_callback(SDL_Window* gwindow,
    SysDouble xoffset, 
    SysDouble yoffset) {

  sys_debug_N("%s", "window_scroll");
}

static void fr_sdl_window_cursor_enter_callback(SDL_Window* gwindow,
    SysInt entered) {
  // sys_debug_N("%s", "cursor_enter");
}

static void fr_sdl_window_maximize_callback(SDL_Window* window, int maximized) {
}

static void fr_sdl_window_framebuffer_size_callback(SDL_Window* gwindow,
    SysInt width, 
    SysInt height) {
  FrWindow *window = fr_sdl_get_window(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_FRAMEBUFFER_RESIZE);

  fr_events_push_head(e);
}

static void fr_sdl_window_focus_callback(SDL_Window* gwindow, SysInt focused) {
  sys_debug_N("window_focus :%d", focused);
}

static void fr_sdl_window_size_callback(SDL_Window* gwindow,
    SysInt width, 
    SysInt height) {
  FrWindow *window = fr_sdl_get_window(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_WINDOW_RESIZE);

  fr_events_push_head(e);
}

static void fr_sdl_window_pos_callback(SDL_Window* gwindow,
    SysInt xpos, 
    SysInt ypos) {
  // sys_debug_N("%s", "window_pos");
}

static void fr_sdl_window_refresh_callback(SDL_Window* gwindow) {
  FrWindow *window = fr_sdl_get_window(gwindow);

  FrEvent *e = fr_event_refresh_new_I(window);

  fr_events_dispatch(e);
}

static void fr_sdl_window_event_unregister(FrSdlWindow *self) {
  sys_return_if_fail(self != NULL);

#if 0
  SDL_Window *gwindow = self->gwindow;

  SDL_SetErrorCallback(NULL);
  SDL_SetWindowFocusCallback(gwindow, NULL);
  SDL_SetWindowPosCallback(gwindow, NULL);
  SDL_SetWindowRefreshCallback(gwindow, NULL);
  SDL_SetFramebufferSizeCallback(gwindow, NULL);
  SDL_SetWindowSizeCallback(gwindow, NULL);
  SDL_SetKeyCallback(gwindow, NULL);
  SDL_SetMouseButtonCallback(gwindow, NULL);
  SDL_SetCursorPosCallback(gwindow, NULL);
  SDL_SetWindowCloseCallback(gwindow, NULL);
  SDL_SetScrollCallback(gwindow, NULL);
  SDL_SetCursorEnterCallback(gwindow, NULL);
  SDL_SetWindowMaximizeCallback(gwindow, NULL);
#endif
}

static void fr_sdl_window_event_register(FrSdlWindow *self) {
  sys_return_if_fail(self != NULL);

#if 0
  SDL_Window *gwindow = self->gwindow;

  SDL_SetErrorCallback(fr_sdl_error_callback);
  SDL_SetWindowFocusCallback(gwindow, fr_sdl_window_focus_callback);
  SDL_SetWindowPosCallback(gwindow, fr_sdl_window_pos_callback);
  SDL_SetWindowRefreshCallback(gwindow, fr_sdl_window_refresh_callback);
  SDL_SetFramebufferSizeCallback(gwindow, fr_sdl_window_framebuffer_size_callback);
  SDL_SetWindowSizeCallback(gwindow, fr_sdl_window_size_callback);
  SDL_SetKeyCallback(gwindow, fr_sdl_window_key_callback);
  SDL_SetMouseButtonCallback(gwindow, fr_sdl_window_mouse_button_callback);
  SDL_SetCursorPosCallback(gwindow, fr_sdl_window_cursor_pos_callback);
  SDL_SetWindowCloseCallback(gwindow, fr_sdl_window_close_callback);
  SDL_SetScrollCallback(gwindow, fr_sdl_window_scroll_callback);
  SDL_SetCursorEnterCallback(gwindow, fr_sdl_window_cursor_enter_callback);
  SDL_SetWindowMaximizeCallback(gwindow, fr_sdl_window_maximize_callback);
#endif
}

static void sdl_handle_event(SDL_Event *e) {
}

/* window setup */
static void fr_delay_i(SysUInt64 msec) {
  SDL_Delay(msec);
}

static void fr_wait_events_timeout_i(SysDouble msec) {
  SDL_Event e;
  SDL_WaitEventTimeout(&e, msec);
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

static SysPointer fr_window_get_native_window (FrWindow* window) {
  FrSdlWindow *self = FR_SDL_WINDOW(window);
  SDL_SysWMinfo info;
  SDL_GetWindowWMInfo(self->gwindow, &info);

#if SYS_OS_WIN32
  return UINT_TO_POINTER(info.info.win.window);
#elif SYS_OS_UNIX
  return UINT_TO_POINTER(info.info.x11.window);
#endif
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

  SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
  SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
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

  fr_sdl_set_window(gwindow, self);
}

static void fr_window_destroy(FrWindow *o) {
  FrSdlWindow *self = FR_SDL_WINDOW(o);

  SDL_DestroyWindow(self->gwindow);
}

static void i_window_imp(FrIWindowInterface *iface) {
  iface->create = fr_sdl_window_create;
  iface->set_error_callback = fr_sdl_set_error_callback;
  iface->get_framebuffer_size = fr_sdl_window_get_framebuffer_size;
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
