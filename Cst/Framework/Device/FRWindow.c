#include <Framework/Device/FRWindowPrivate.h>
#include <Framework/Device/FRDisplay.h>
#include <Framework/Event/FREventCore.h>


#define fr_glfw_get_window(gwindow) (FRWindow *)glfwGetWindowUserPointer(gwindow)
#define fr_glfw_set_window(gwindow, window) glfwSetWindowUserPointer(gwindow, window)

static void fr_window_event_setup(FRWindow *self);
static void fr_window_event_teardown(FRWindow *self);

SYS_DEFINE_TYPE_WITH_PRIVATE(FRWindow, fr_window, SYS_TYPE_OBJECT);

static GLFWwindow* fr_window_create_window_i(SysInt width, SysInt height, const SysChar *title, GLFWwindow *gshare) {
  GLFWwindow *gwindow;
  SYS_LEAK_IGNORE_BEGIN;
  gwindow = glfwCreateWindow(width, height, title, NULL, gshare);
  SYS_LEAK_IGNORE_END;

  return gwindow;
}

FRDisplay *fr_window_get_display(FRWindow *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRWindowPrivate* priv = self->priv;

  return priv->display;
}

FR_WINDOW_BACKEND_ENUM fr_window_backend(FRWindow *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return FR_WINDOW_GLFW;
}

GLFWwindow *fr_window_real_window(FRWindow *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRWindowPrivate* priv = self->priv;

  return priv->gwindow;
}

FRWindow *fr_window_top_new(FRDisplay *display) {
  sys_return_val_if_fail(display != NULL, NULL);

  FRWindow *window = fr_window_new_I(display, NULL);
  if (window == NULL) {
    sys_error_N("%s", SYS_("Create top window failed."));
    return NULL;
  }

  return window;
}

FRMonitor* fr_get_primary_monitor(void) {
  return glfwGetPrimaryMonitor();
}

void fr_window_get_framebuffer_size(FRWindow *self, SysInt *width, SysInt * height) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;

  glfwGetFramebufferSize(priv->gwindow, width, height);
}

void fr_window_set_size(FRWindow *self, SysInt width, SysInt height) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;

  glfwSetWindowSize(priv->gwindow, width, height);
}

void fr_window_set_title(FRWindow *self, SysChar *title) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;

  glfwSetWindowTitle(priv->gwindow, title);
}

void fr_window_set_opacity(FRWindow *self, double opacity) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;

  glfwSetWindowOpacity(priv->gwindow, opacity);
}

void fr_window_get_size(FRWindow *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;

  glfwGetWindowSize(priv->gwindow, width, height);
}

static void fr_window_error_callback(SysInt error_code, const char* description) {
  sys_error_N(SYS_("Error: %d, %s"), error_code, description);
}

SysInt fr_window_get_key(FRWindow *self, SysInt key) {
  sys_return_val_if_fail(self != NULL, -1);

  FRWindowPrivate* priv = self->priv;

  return glfwGetKey(priv->gwindow, key);
}

const SysChar* fr_key_get_name(SysInt key, SysInt scancode) {
  return glfwGetKeyName(key, scancode);
}

/* event callbacks */
static void fr_window_key_callback(GLFWwindow* gwindow, SysInt key, SysInt scancode, SysInt action, SysInt mods) {
  FRWindow *self = fr_glfw_get_window(gwindow);

  FREvent *e = fr_event_key_new_I(self, key, scancode, action, mods);

  fr_events_push_head(e);
}

static void fr_window_mouse_button_callback(GLFWwindow* gwindow, SysInt button, SysInt action, SysInt mods) {
  FRWindow *self = fr_glfw_get_window(gwindow);

  FREvent *e = fr_event_mousekey_new_I(self, button, action, mods);

  fr_events_push_head(e);
}

static void fr_window_cursor_pos_callback(GLFWwindow* gwindow, SysDouble xpos, SysDouble ypos) {
  FRWindow *self = fr_glfw_get_window(gwindow);

  FREvent *e = fr_event_cursor_move_new_I(self, xpos, ypos);

  fr_events_push_head(e);
}

static void fr_window_close_callback(GLFWwindow* gwindow) {
  glfwSetWindowShouldClose(gwindow, 1);

  FRWindow *self = fr_glfw_get_window(gwindow);

  FREvent *e = fr_event_any_new_I(self, FR_EVENT_TWINDOW_CLOSE);

  fr_events_push_head(e);
}

static void fr_window_scroll_callback(GLFWwindow* gwindow, SysDouble xoffset, SysDouble yoffset) {
  sys_debug_N("%s", "window_scroll");
}

static void fr_window_cursor_enter_callback(GLFWwindow* gwindow, SysInt entered) {
  // sys_debug_N("%s", "cursor_enter");
}

static void fr_window_maximize_callback(GLFWwindow* window, int maximized) {
}

static void fr_window_framebuffer_size_callback(GLFWwindow* gwindow, SysInt width, SysInt height) {
  FRWindow *self = fr_glfw_get_window(gwindow);

  FREventAny *e = fr_event_any_new_I(self, FR_EVENT_TFRAMEBUFFER_RESIZE);

  fr_events_push_head(e);
}

static void fr_window_focus_callback(GLFWwindow* gwindow, SysInt focused) {
  sys_debug_N("%s", "window_focus");
}

static void fr_window_size_callback(GLFWwindow* gwindow, SysInt width, SysInt height) {
  FRWindow *self = fr_glfw_get_window(gwindow);

  FREventAny *e = fr_event_any_new_I(self, FR_EVENT_TWINDOW_RESIZE);

  fr_events_push_head(e);
}

static void fr_window_pos_callback(GLFWwindow* gwindow, SysInt xpos, SysInt ypos) {
  // sys_debug_N("%s", "window_pos");
}

static void fr_window_refresh_callback(GLFWwindow* gwindow) {
  FRWindow *self = fr_glfw_get_window(gwindow);

  FREvent *e = fr_event_any_new_I(self, FR_EVENT_TWINDOW_REFRESH);

  fr_events_dispatch(e);
}

static void fr_window_event_teardown(FRWindow *self) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;
  GLFWwindow *gwindow = priv->gwindow;

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

static void fr_window_event_setup(FRWindow *self) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;
  GLFWwindow *gwindow = priv->gwindow;

  glfwSetErrorCallback(fr_window_error_callback);
  glfwSetWindowFocusCallback(gwindow, fr_window_focus_callback);
  glfwSetWindowPosCallback(gwindow, fr_window_pos_callback);
  glfwSetWindowRefreshCallback(gwindow, fr_window_refresh_callback);
  glfwSetFramebufferSizeCallback(gwindow, fr_window_framebuffer_size_callback);
  glfwSetWindowSizeCallback(gwindow, fr_window_size_callback);
  glfwSetKeyCallback(gwindow, fr_window_key_callback);
  glfwSetMouseButtonCallback(gwindow, fr_window_mouse_button_callback);
  glfwSetCursorPosCallback(gwindow, fr_window_cursor_pos_callback);
  glfwSetWindowCloseCallback(gwindow, fr_window_close_callback);
  glfwSetScrollCallback(gwindow, fr_window_scroll_callback);
  glfwSetCursorEnterCallback(gwindow, fr_window_cursor_enter_callback);
  glfwSetWindowMaximizeCallback(gwindow, fr_window_maximize_callback);
}

/* window setup */
SysPointer fr_window_get_data(FRWindow * self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRWindowPrivate* priv = self->priv;

  return priv->user_data;
}

void fr_window_set_data(FRWindow *self, SysPointer data) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;

  priv->user_data = data;
}

void fr_wait_events(void) {
  SYS_LEAK_IGNORE_BEGIN;
    glfwWaitEvents();
  SYS_LEAK_IGNORE_END;
}

void fr_post_empty_events(void) {
  SYS_LEAK_IGNORE_BEGIN;
    glfwPostEmptyEvent();
  SYS_LEAK_IGNORE_END;
}

void fr_poll_events(void) {
  SYS_LEAK_IGNORE_BEGIN;
    glfwPollEvents();
  SYS_LEAK_IGNORE_END;
}

void fr_window_swap_buffers(FRWindow *self) {
  sys_return_if_fail(self != NULL);

  FRWindowPrivate* priv = self->priv;

  SYS_LEAK_IGNORE_BEGIN;
    glfwSwapBuffers(priv->gwindow);
  SYS_LEAK_IGNORE_END;
}

void fr_window_teardown(void) {
  SYS_LEAK_IGNORE_BEGIN;
    glfwTerminate();
  SYS_LEAK_IGNORE_END;
}

void fr_window_setup(void) {
  SYS_LEAK_IGNORE_BEGIN;
    if (!glfwInit()) {
      sys_error_N("%s", SYS_("GFLW failed to init"));
      fr_window_teardown();
    }
  SYS_LEAK_IGNORE_END;
#if FR_GL_API 
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#else

#if USE_VULKAN
  SYS_LEAK_IGNORE_BEGIN;
    if (!glfwVulkanSupported()) {
      sys_error_N("%s", SYS_("glfwVulkanSupported return false."));
      fr_window_deinit();
    }
  SYS_LEAK_IGNORE_END;
#endif

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#endif
}

#if defined(VK_VERSION_1_0)
void fr_window_create_vk_surface(FRWindow *self, VkInstance instance, VkSurfaceKHR *surfacekhr) {
  if (glfwCreateWindowSurface(instance, priv->gwindow, NULL, surfacekhr) != VK_SUCCESS) {
    sys_error_N("%s", SYS_("failed to create vulkan surface"));
  }
}
#endif

/* object api */
void fr_window_construct(SysObject *o, FRDisplay *display, FRWindow *share) {

  FRWindow* self = FR_WINDOW(o);
  FRWindowPrivate* priv = self->priv;

  GLFWwindow *gwindow = NULL;
  GLFWwindow *gshare;
  const SysChar *name;

  name = sys_exe_path();

  if (share != NULL) {
    gshare = share->priv->gwindow;

  } else {
    gshare = NULL;
  }

  priv->display = display;
  sys_object_ref(display);

  priv->share = share;
  gwindow = fr_window_create_window_i(800, 600, name, gshare);

  priv->gwindow = gwindow;
  fr_glfw_set_window(gwindow, self);
  fr_window_event_setup(self);
}

FRWindow* fr_window_new(void) {
  return sys_object_new(FR_TYPE_WINDOW, NULL);
}

FRWindow *fr_window_new_I(FRDisplay *display, FRWindow *share) {
  sys_return_val_if_fail(display != NULL, NULL);

  FRWindow *o = fr_window_new();

  fr_window_construct(SYS_OBJECT(o), display, share);

  return o;
}

static void fr_window_dispose(SysObject* o) {
  FRWindow *self = FR_WINDOW(o);
  FRWindowPrivate* priv = self->priv;

  fr_glfw_set_window(priv->gwindow, NULL);
  fr_window_event_teardown(self);

  SYS_LEAK_IGNORE_BEGIN;
  glfwDestroyWindow(priv->gwindow);
  SYS_LEAK_IGNORE_END;

  sys_object_unref(priv->display);

  SYS_OBJECT_CLASS(fr_window_parent_class)->dispose(o);
}

static void fr_window_class_init(FRWindowClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_window_construct;
  ocls->dispose = fr_window_dispose;
}

void fr_window_init(FRWindow *self) {
  self->priv = fr_window_get_private(self);
}
