#include <Framework/Device/FrIWindow.h>
#include <Framework/Device/FrGlfwWindow.h>
#include <Framework/Device/FrSdlWindow.h>

static FrIWindowInterface self = {0};

SYS_DEFINE_INTERFACE(FrIWindow, fr_i_window, SYS_TYPE_OBJECT);

void fr_i_window_setup(void) {
#if USE_GLFW
  fr_glfw_window_setup();
  fr_glfw_window_iface_setup(&self);
#elif USE_SDL
  fr_sdl_window_setup();
  fr_sdl_window_iface_setup(&self);
#else

  sys_error_N("%s", "glfw or sdl backend should set when compile");
#endif
}

void fr_i_window_teardown(void) {
#if USE_GLFW
  fr_glfw_window_teardown();
#elif USE_SDL
  fr_sdl_window_teardown();
#else

  sys_error_N("%s", "glfw or sdl backend should set when compile");
#endif
}
/* object api */
void fr_i_window_default_init(FrIWindowInterface* iface) {
}

void fr_i_window_create (
    FrWindow* window,
    SysInt width,
    SysInt height,
    const SysChar *title,
    FrWindow *share) {

  self.create(window, width, height, title, share);
}

void fr_i_window_display_create (FrDisplay *display) {

  self.display_create(display);
}

void fr_i_window_set_error_callback (FrWindowErrFunc callback) {

  self.set_error_callback(callback);
}

void fr_i_window_get_framebuffer_size (FrRender *render, SysInt *width, SysInt *height) {

  self.get_framebuffer_size(render, width, height);
}

void fr_i_window_window_get_size (FrWindow *window, SysInt *width, SysInt *height) {

  self.window_get_size(window, width, height);
}

void fr_i_window_window_set_size (FrWindow *window, SysInt width, SysInt height) {

  self.window_set_size(window, width, height);
}

void fr_i_window_window_set_title (FrWindow *window, const SysChar *title) {

  self.window_set_title(window, title);
}

void fr_i_window_window_set_opacity (FrWindow *window, SysDouble opacity) {

  self.window_set_opacity(window, opacity);
}

void fr_i_window_window_set_should_close (FrWindow *window, SysBool bvalue) {

  self.window_set_should_close(window, bvalue);
}

void fr_i_window_swap_buffers (FrWindow *window) {

  self.swap_buffers(window);
}

void fr_i_window_window_destroy (FrWindow *window) {

  self.window_destroy(window);
}

const SysChar *fr_i_window_get_key_name (SysInt key, SysInt scancode) {

  return self.get_key_name(key, scancode);
}

SysInt fr_i_window_get_key (FrWindow *window, SysInt key) {

  return self.get_key(window, key);
}

void fr_i_window_wait_events_timeout (SysInt msec) {

  self.wait_events_timeout(msec);
}

void fr_i_window_wait_events (void) {

  self.wait_events();
}

void fr_i_window_post_empty_event (void) {

  self.post_empty_event();
}

void fr_i_window_poll_events (void) {

  self.poll_events();
}

void fr_i_window_delay (SysUInt ms) {

  self.delay(ms);
}

SysPointer fr_i_window_get_native_window (FrWindow* window) {

  return self.get_native_window(window);
}

SysPointer fr_i_window_get_native_display (FrDisplay *display) {

  return self.get_native_display(display);
}
