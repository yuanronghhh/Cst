#include <Framework/Device/FrWindow.h>
#include <Framework/Device/FrGlfwWindow.h>
#include <Framework/Device/FrSdlWindow.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrIDevice.h>
#include <Framework/Event/FrEventCore.h>

static SysType g_type = 0;

SYS_DEFINE_TYPE(FrWindow, fr_window, FR_TYPE_DEVICE);

FrDisplay *fr_window_get_display(FrWindow *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->display;
}

FR_WINDOW_BACKEND_ENUM fr_window_backend(FrWindow *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return FR_WINDOW_GLFW;
}

FrWindow *fr_window_top_new(FrDisplay *display) {
  sys_return_val_if_fail(display != NULL, NULL);

  FrWindow *window = fr_window_new_I(display, NULL);
  if (window == NULL) {
    sys_error_N("%s", SYS_("Create top window failed."));
    return NULL;
  }

  return window;
}

/* window setup */
void fr_window_setup (void) {
  fr_i_window_setup();

#if USE_GLFW
  g_type = FR_TYPE_GLFW_WINDOW;
#elif USE_SDL
  g_type = FR_TYPE_SDL_WINDOW;
#else

  sys_error_N("%s", "glfw or sdl backend should set when compile");
#endif
}

void fr_window_teardown (void) {
  fr_i_window_teardown();
}

SysPointer fr_window_get_data(FrWindow * self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->user_data;
}

void fr_window_set_data(FrWindow *self, SysPointer data) {
  sys_return_if_fail(self != NULL);

  self->user_data = data;
}

/* object api */
static void fr_window_construct(
    FrWindow *self,
    FrWindowContext *info) {

  const SysChar *title = info->title == NULL ? sys_exe_path(): info->title;

  self->display = sys_object_ref(info->display);
  self->share = info->share;

  fr_i_window_create(self, 800, 600, title, info->share);
}

FrWindow* fr_window_new(void) {

  return sys_object_new(g_type, NULL);
}

FrWindow *fr_window_new_I(FrDisplay *display, FrWindow *share) {
  sys_return_val_if_fail(display != NULL, NULL);

  FrWindowContext info = {0};
  info.display = display;
  info.share = share;

  FrWindow *o = fr_window_new();

  fr_window_construct(o, &info);

  return o;
}

static void fr_window_dispose(SysObject* o) {
  FrWindow *self = FR_WINDOW(o);

  sys_clear_pointer(&self->display, _sys_object_unref);
  fr_i_window_destroy_window(self);

  SYS_OBJECT_CLASS(fr_window_parent_class)->dispose(o);
}

static void fr_window_class_init(FrWindowClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_window_dispose;
}

void fr_window_init(FrWindow *self) {

  self->window_type = FR_TYPE_WINDOW;
}
