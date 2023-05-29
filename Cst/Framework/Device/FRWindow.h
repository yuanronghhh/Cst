#ifndef __FR_WINDOW_H__
#define __FR_WINDOW_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_WINDOW (fr_window_get_type())
#define FR_WINDOW(o) ((FRWindow* )sys_object_cast_check(o, FR_TYPE_WINDOW))
#define FR_WINDOW_CLASS(o) ((FRWindowClass *)sys_class_cast_check(o, FR_TYPE_WINDOW))
#define FR_WINDOW_GET_CLASS(o) sys_instance_get_class(o, FRWindowClass)

struct _FRWindowClass {
  SysObjectClass parent;
};

struct _FRWindow {
  SysObject parent;
  FRWindowPrivate *priv;
};

SYS_API void fr_window_setup(void);
SYS_API void fr_window_teardown(void);

SYS_API SysType fr_window_get_type(void);
SYS_API FRWindow* fr_window_new_I (FRDisplay* display, FRWindow* share);

SYS_API FR_WINDOW_BACKEND_ENUM fr_window_backend(FRWindow *window);
SYS_API FRDisplay* fr_window_get_display (FRWindow* window);
SYS_API void fr_window_set_title (FRWindow* window, SysChar* title);
SYS_API void fr_window_set_size (FRWindow* window, SysInt width, SysInt height);
SYS_API void fr_window_get_size (FRWindow* window, SysInt* width, SysInt* height);
SYS_API FRWindow* fr_window_top_new (FRDisplay* display);
SYS_API void fr_window_set_data (FRWindow* window, SysPointer data);
SYS_API SysPointer fr_window_get_data (FRWindow* window);
SYS_API FRMonitor* fr_get_primary_monitor (void);
SYS_API void fr_window_get_framebuffer_size (FRWindow* window, SysInt* width, SysInt* height);
SYS_API void fr_window_swap_buffers (FRWindow* window);
SYS_API void fr_wait_events (void);
SYS_API void fr_post_empty_events (void);
SYS_API void fr_poll_events (void);
SYS_API GLFWwindow* fr_window_real_window (FRWindow* window);
SYS_API SysInt fr_window_get_key (FRWindow *window, SysInt key);
SYS_API const SysChar* fr_key_get_name(SysInt key, SysInt scancode);

#if SYS_OS_WIN32
  HWND fr_window_get_win32_window(FRWindow* window);
#elif SYS_OS_UNIX
  SYS_API Window fr_window_get_x11_window(FRWindow* window);
#endif

#if defined(VK_VERSION_1_0)
void fr_window_create_vk_surface(FRWindow* window, VkInstance instance, VkSurfaceKHR* surfacekhr);
#endif

SYS_END_DECLS

#endif

