#ifndef __FR_WINDOW_H__
#define __FR_WINDOW_H__

#include <Framework/Device/FrDevice.h>

SYS_BEGIN_DECLS

#define FR_TYPE_WINDOW (fr_window_get_type())
#define FR_WINDOW(o) ((FrWindow* )sys_object_cast_check(o, FR_TYPE_WINDOW))
#define FR_WINDOW_CLASS(o) ((FrWindowClass *)sys_class_cast_check(o, FR_TYPE_WINDOW))
#define FR_WINDOW_GET_CLASS(o) sys_instance_get_class(o, FrWindowClass)

struct _FrWindowClass {
  FrDeviceClass parent;
};

struct _FrWindow {
  FrDevice parent;

  /* <private> */
  GLFWwindow* gwindow;
  SysPointer user_data;
  FrDisplay* display;

  SysChar* title;
  FrWindow* share;
};

SYS_API void fr_window_setup(void);
SYS_API void fr_window_teardown(void);

SYS_API SysType fr_window_get_type(void);
SYS_API FrWindow* fr_window_new_I (FrDisplay* display, FrWindow* share);

SYS_API FR_WINDOW_BACKEND_ENUM fr_window_backend(FrWindow *window);
SYS_API FrDisplay* fr_window_get_display (FrWindow* window);
SYS_API void fr_window_set_title (FrWindow* window, SysChar* title);
SYS_API void fr_window_set_opacity(FrWindow *self, double opacity);
SYS_API void fr_window_set_size (FrWindow* window, SysInt width, SysInt height);
SYS_API void fr_window_get_size (FrWindow* window, SysInt* width, SysInt* height);
SYS_API FrWindow* fr_window_top_new (FrDisplay* display);
SYS_API void fr_window_set_data (FrWindow* window, SysPointer data);
SYS_API SysPointer fr_window_get_data (FrWindow* window);
SYS_API FRMonitor* fr_get_primary_monitor (void);
SYS_API void fr_window_get_framebuffer_size (FrWindow* window, SysInt* width, SysInt* height);
SYS_API void fr_window_swap_buffers (FrWindow* window);
SYS_API void fr_wait_events (void);
SYS_API void fr_post_empty_events (void);
SYS_API void fr_poll_events (void);
SYS_API GLFWwindow* fr_window_real_window (FrWindow* window);
SYS_API SysInt fr_window_get_key (FrWindow *window, SysInt key);
SYS_API const SysChar* fr_key_get_name(SysInt key, SysInt scancode);

#if SYS_OS_WIN32
  SYS_API HWND fr_window_get_win32_window(FrWindow* window);
#elif SYS_OS_UNIX
  SYS_API Window fr_window_get_x11_window(FrWindow* window);
#endif

#if defined(VK_VERSION_1_0)
SYS_API void fr_window_create_vk_surface(FrWindow* window, VkInstance instance, VkSurfaceKHR* surfacekhr);
#endif

void fr_window_set_gwindow(FrWindow *self, GLFWwindow * gwindow);
SYS_API GLFWwindow * fr_window_get_gwindow(FrWindow *self);

SYS_END_DECLS

#endif

