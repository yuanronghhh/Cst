#include <Framework/Device/FrGlfwWindow.h>
#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrIDevice.h>
#include <Framework/Event/FrEventCore.h>

#define fr_glfw_get_window(gwindow) ((FrWindow *)glfwGetWindowUserPointer(gwindow))
#define fr_glfw_set_window(gwindow, window) glfwSetWindowUserPointer(gwindow, window)

static void fr_glfw_window_event_register(FrGlfwWindow *self);
static void fr_glfw_window_event_unregister(FrGlfwWindow *self);

SYS_DEFINE_TYPE(FrGlfwWindow, fr_glfw_window, FR_TYPE_WINDOW);

static GLFWwindow* fr_glfw_window_create_i(
    SysInt width,
    SysInt height,
    const SysChar *title,
    GLFWwindow *gshare) {
  GLFWwindow *gwindow;

  SYS_LEAK_IGNORE_BEGIN;
  gwindow = glfwCreateWindow(width, height, title, NULL, gshare);
  SYS_LEAK_IGNORE_END;

  return gwindow;
}

FR_WINDOW_BACKEND_ENUM fr_glfw_window_backend() {

  return FR_WINDOW_GLFW;
}

FrMonitor* fr_glfw_get_primary_monitor(void) {
  return glfwGetPrimaryMonitor();
}

static void fr_glfw_window_get_framebuffer_size(
    FrWindow *window,
    SysInt *width, 
    SysInt * height) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_if_fail(window != NULL);

  glfwGetFramebufferSize(self->gwindow, width, height);
}

static void fr_glfw_window_set_size(FrWindow *window, SysInt width, SysInt height) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_if_fail(self != NULL);

  glfwSetWindowSize(self->gwindow, width, height);
}

static void fr_glfw_window_set_title(FrWindow *window, const SysChar *title) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_if_fail(self != NULL);

  glfwSetWindowTitle(self->gwindow, title);
}

static void fr_glfw_window_set_opacity(FrWindow *window, double opacity) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_if_fail(self != NULL);

  glfwSetWindowOpacity(self->gwindow, (float)opacity);
}

static void fr_glfw_window_set_should_close (FrWindow *window, SysBool bvalue) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);

  glfwSetWindowShouldClose(self->gwindow, bvalue);
}

static void fr_glfw_window_destroy (FrWindow *window) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);

  glfwDestroyWindow(self->gwindow);
}

static void fr_glfw_window_get_size(FrWindow *window, SysInt *width, SysInt *height) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_if_fail(self != NULL);

  glfwGetWindowSize(self->gwindow, width, height);
}

static void fr_glfw_error_callback(
    SysInt error_code, 
    const char* description) {

  sys_error_N(SYS_("Error: %d, %s"), error_code, description);
}

static void fr_glfw_set_error_callback (FrWindowErrFunc callback) {

  glfwSetErrorCallback(callback);
}

SysInt fr_glfw_get_key(FrWindow *window, SysInt key) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_val_if_fail(self != NULL, -1);

  return glfwGetKey(self->gwindow, key);
}

const SysChar* fr_glfw_get_key_name(SysInt key, SysInt scancode) {
  return glfwGetKeyName(key, scancode);
}

void fr_glfw_window_set_gwindow(FrWindow *window, GLFWwindow * gwindow) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_if_fail(self != NULL);

  self->gwindow = gwindow;
}

GLFWwindow * fr_glfw_window_get_gwindow(FrWindow *window) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);
  sys_return_val_if_fail(self != NULL, NULL);

  return self->gwindow;
}

/* event callbacks */
static void fr_glfw_window_key_callback(GLFWwindow* gwindow,
    SysInt key, 
    SysInt scancode, 
    SysInt action, 
    SysInt mods) {
  FrWindow *window = fr_glfw_get_window(gwindow);

  FrEvent *e = fr_event_key_new_I(window, key, scancode, action, mods);

  fr_events_push_head(e);
}

static void fr_glfw_window_mouse_button_callback(GLFWwindow* gwindow,
    SysInt button, 
    SysInt action, 
    SysInt mods) {
  FrWindow *window = fr_glfw_get_window(gwindow);

  FrEvent *e = fr_event_mousekey_new_I(window, button, action, mods);

  fr_events_push_head(e);
}

static void fr_glfw_window_cursor_pos_callback(GLFWwindow* gwindow,
    SysDouble xpos, 
    SysDouble ypos) {
  FrWindow *window = fr_glfw_get_window(gwindow);

  FrEvent *e = fr_event_cursor_move_new_I(window, xpos, ypos);

  fr_events_push_head(e);
}

static void fr_glfw_window_close_callback(GLFWwindow* gwindow) {
  glfwSetWindowShouldClose(gwindow, 1);

  FrWindow *window = fr_glfw_get_window(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_WINDOW_CLOSE);

  fr_events_push_head(e);
}

static void fr_glfw_window_scroll_callback(GLFWwindow* gwindow,
    SysDouble xoffset, 
    SysDouble yoffset) {
  sys_debug_N("%s", "window_scroll");
}

static void fr_glfw_window_cursor_enter_callback(GLFWwindow* gwindow,
    SysInt entered) {
  // sys_debug_N("%s", "cursor_enter");
}

static void fr_glfw_window_maximize_callback(GLFWwindow* window, int maximized) {
}

static void fr_glfw_window_framebuffer_size_callback(GLFWwindow* gwindow,
    SysInt width, 
    SysInt height) {
  FrWindow *window = fr_glfw_get_window(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_FRAMEBUFFER_RESIZE);

  fr_events_push_head(e);
}

static void fr_glfw_window_focus_callback(GLFWwindow* gwindow, SysInt focused) {
  sys_debug_N("window_focus :%d", focused);
}

static void fr_glfw_window_size_callback(GLFWwindow* gwindow,
    SysInt width, 
    SysInt height) {
  FrWindow *window = fr_glfw_get_window(gwindow);

  FrEvent *e = fr_event_any_new_I(window, FR_EVENT_T_WINDOW_RESIZE);

  fr_events_push_head(e);
}

static void fr_glfw_window_pos_callback(GLFWwindow* gwindow,
    SysInt xpos, 
    SysInt ypos) {
  // sys_debug_N("%s", "window_pos");
}

static void fr_glfw_window_refresh_callback(GLFWwindow* gwindow) {
  FrWindow *window = fr_glfw_get_window(gwindow);

  FrEvent *e = fr_event_refresh_new_I(window);

  fr_events_dispatch(e);
}

static void fr_glfw_window_event_unregister(FrGlfwWindow *self) {
  sys_return_if_fail(self != NULL);

  GLFWwindow *gwindow = self->gwindow;

  glfwSetErrorCallback(NULL);
  glfwSetWindowFocusCallback(gwindow, NULL);
  glfwSetWindowPosCallback(gwindow, NULL);
  glfwSetWindowRefreshCallback(gwindow, NULL);
  glfwSetFramebufferSizeCallback(gwindow, NULL);
  glfwSetWindowSizeCallback(gwindow, NULL);
  glfwSetKeyCallback(gwindow, NULL);
  glfwSetMouseButtonCallback(gwindow, NULL);
  glfwSetCursorPosCallback(gwindow, NULL);
  glfwSetWindowCloseCallback(gwindow, NULL);
  glfwSetScrollCallback(gwindow, NULL);
  glfwSetCursorEnterCallback(gwindow, NULL);
  glfwSetWindowMaximizeCallback(gwindow, NULL);
}

static void fr_glfw_window_event_register(FrGlfwWindow *self) {
  sys_return_if_fail(self != NULL);

  GLFWwindow *gwindow = self->gwindow;

  glfwSetErrorCallback(fr_glfw_error_callback);
  glfwSetWindowFocusCallback(gwindow, fr_glfw_window_focus_callback);
  glfwSetWindowPosCallback(gwindow, fr_glfw_window_pos_callback);
  glfwSetWindowRefreshCallback(gwindow, fr_glfw_window_refresh_callback);
  glfwSetFramebufferSizeCallback(gwindow, fr_glfw_window_framebuffer_size_callback);
  glfwSetWindowSizeCallback(gwindow, fr_glfw_window_size_callback);
  glfwSetKeyCallback(gwindow, fr_glfw_window_key_callback);
  glfwSetMouseButtonCallback(gwindow, fr_glfw_window_mouse_button_callback);
  glfwSetCursorPosCallback(gwindow, fr_glfw_window_cursor_pos_callback);
  glfwSetWindowCloseCallback(gwindow, fr_glfw_window_close_callback);
  glfwSetScrollCallback(gwindow, fr_glfw_window_scroll_callback);
  glfwSetCursorEnterCallback(gwindow, fr_glfw_window_cursor_enter_callback);
  glfwSetWindowMaximizeCallback(gwindow, fr_glfw_window_maximize_callback);
}

/* window */
static void fr_wait_events_timeout_i(SysDouble sec) {
  glfwWaitEventsTimeout(sec);
}

static void fr_wait_events_i(void) {
  glfwWaitEvents();
}

static void fr_post_empty_event_i(void) {
  glfwPostEmptyEvent();
}

static void fr_poll_events_i(void) {
  glfwPollEvents();
}

static void fr_swap_buffers_i(FrWindow *o) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(o);

  sys_return_if_fail(self != NULL);
  glfwSwapBuffers(self->gwindow);
}

static void fr_window_hint (SysInt p, SysInt v) {

  glfwWindowHint(p, v);
}

static SysPointer fr_window_get_native_window (FrWindow* window) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(window);

#if SYS_OS_WIN32
  return UINT_TO_POINTER(glfwGetWin32Window(self->gwindow));
#elif SYS_OS_UNIX
  return UINT_TO_POINTER(glfwGetX11Window(self->gwindow));
#endif
}

void fr_glfw_window_teardown(void) {

  glfwTerminate();
}

void fr_glfw_window_setup(void) {
  SYS_LEAK_IGNORE_BEGIN;
  if (!glfwInit()) {
    sys_error_N("%s", SYS_("GFLW failed to init"));
    glfwTerminate();
  }
  SYS_LEAK_IGNORE_END;

#if FR_GL_API 
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#else

#if USE_VULKAN
  if (!glfwVulkanSupported()) {
    sys_error_N("%s", SYS_("glfwVulkanSupported return false."));
    fr_glfw_window_deinit();
  }
#endif

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#endif
}

#if defined(VK_VERSION_1_0)
void fr_glfw_window_create_vk_surface(FrWindow *window,
    VkInstance instance, 
    VkSurfaceKHR *surfacekhr) {
  if (glfwCreateWindowSurface(instance,
        self->gwindow, 
        NULL, 
        surfacekhr) != VK_SUCCESS) {

    sys_error_N("%s", SYS_("failed to create vulkan surface"));
  }
}
#endif

static void fr_glfw_window_create(
    FrWindow *o,
    SysInt width,
    SysInt height,
    const SysChar *title,
    FrWindow *share) {

  FrGlfwWindow *self = FR_GLFW_WINDOW(o);
  FrGlfwWindow *sshare = FR_GLFW_WINDOW(share);
  GLFWwindow *gwindow = NULL;
  GLFWwindow *gshare = share == NULL ? NULL : sshare->gwindow;

  gwindow = fr_glfw_window_create_i(800, 600, title, gshare);
  self->gwindow = gwindow;

  fr_glfw_set_window(gwindow, self);
  fr_glfw_window_event_register(self);
}

static void i_window_imp(FrIWindowInterface *iface) {
  iface->create = fr_glfw_window_create;
  iface->set_error_callback = fr_glfw_set_error_callback;
  iface->get_framebuffer_size = fr_glfw_window_get_framebuffer_size;
  iface->window_get_size = fr_glfw_window_get_size;
  iface->window_set_size = fr_glfw_window_set_size;
  iface->window_set_title = fr_glfw_window_set_title;
  iface->window_set_opacity = fr_glfw_window_set_opacity;
  iface->window_set_should_close = fr_glfw_window_set_should_close;
  iface->window_destroy = fr_glfw_window_destroy;
  iface->swap_buffers = fr_swap_buffers_i;
  iface->get_key_name = fr_glfw_get_key_name;
  iface->get_key = fr_glfw_get_key;
  iface->wait_events_timeout = fr_wait_events_timeout_i;
  iface->wait_events = fr_wait_events_i;
  iface->post_empty_event = fr_post_empty_event_i;
  iface->poll_events = fr_poll_events_i;
  iface->get_native_window = fr_window_get_native_window;
}

void fr_glfw_window_iface_setup(FrIWindowInterface *iface) {

  i_window_imp(iface);
}

/* object api */
static void fr_window_destroy_i(SysObject *o) {
  FrGlfwWindow *self = FR_GLFW_WINDOW(o);

  fr_glfw_set_window(self->gwindow, NULL);
  fr_glfw_window_event_unregister(self);

  glfwDestroyWindow(self->gwindow);
}

static void fr_glfw_window_dispose(SysObject* o) {

  fr_window_destroy_i(o);
}

static void fr_glfw_window_class_init(FrGlfwWindowClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_glfw_window_dispose;
}

void fr_glfw_window_init(FrGlfwWindow* self) {
}
