#ifndef __FR_WINDOW_H__
#define __FR_WINDOW_H__

#include <Framework/Device/FrDevice.h>
#include <Framework/Device/FrIWindow.h>

SYS_BEGIN_DECLS

#define FR_TYPE_WINDOW (fr_window_get_type())
#define FR_WINDOW(o) ((FrWindow* )sys_object_cast_check(o, FR_TYPE_WINDOW))
#define FR_WINDOW_CLASS(o) ((FrWindowClass *)sys_class_cast_check(o, FR_TYPE_WINDOW))
#define FR_WINDOW_GET_CLASS(o) sys_instance_get_class(o, FrWindowClass)

struct _FrWindowClass {
  FrDeviceClass parent;

  void (*construct) (FrWindow *self, FrWindowContext *info);
};

struct _FrWindow {
  FrDevice parent;

  /* <private> */
  SysPointer user_data;
  FrDisplay* display;

  FrWindow* share;
  SysType window_type;
};

struct _FrWindowContext {
  FrDisplay* display;
  SysChar* title;
  FrWindow* share;
  SysType window_type;
};

SYS_API SysType fr_window_get_type(void);
FrWindow *fr_window_new_I(FrDisplay *display, FrWindow *share);

void fr_window_setup (void);
void fr_window_teardown (void);

SYS_API FR_WINDOW_BACKEND_ENUM fr_window_backend(FrWindow *window);
SYS_API FrDisplay* fr_window_get_display (FrWindow* window);
SYS_API FrWindow *fr_window_top_new(FrDisplay *display);
SYS_API void fr_window_set_data(FrWindow *self, SysPointer data);
SYS_API SysPointer fr_window_get_data(FrWindow * self);

#define fr_window_set_title fr_i_window_window_set_title
#define fr_window_set_opacity fr_i_window_set_window_opacity
#define fr_window_set_size fr_i_window_window_set_size
#define fr_window_get_size fr_i_window_window_get_size
#define fr_window_get_framebuffer_size fr_i_window_get_framebuffer_size
#define fr_swap_buffers fr_i_window_swap_buffers
#define fr_wait_events fr_i_window_wait_events
#define fr_wait_events_timeout fr_i_window_wait_events_timeout
#define fr_post_empty_event fr_i_window_post_empty_event
#define fr_poll_events fr_i_window_poll_events
#define fr_delay fr_i_window_delay
#define fr_window_get_key fr_i_window_get_key
#define fr_window_get_key_name fr_i_window_get_key_name
#define fr_window_audio_open fr_i_window_audio_open
#define fr_window_audio_resume fr_i_window_audio_resume
#define fr_window_audio_close fr_i_window_audio_close
#define fr_window_audio_stream_create fr_i_window_audio_stream_create
#define fr_window_audio_stream_put_data fr_i_window_audio_stream_put_data
#define fr_window_audio_stream_free fr_i_window_audio_stream_free
#define fr_window_audio_get_info fr_i_window_audio_get_info

#if SYS_OS_WIN32
  SYS_API HWND fr_window_get_win32_window(FrWindow* window);
#elif SYS_OS_UNIX
  SYS_API Window fr_window_get_x11_window(FrWindow* window);
#endif

#if defined(VK_VERSION_1_0)
SYS_API void fr_window_create_vk_surface(FrWindow* window, VkInstance instance, VkSurfaceKHR* surfacekhr);
#endif

SYS_END_DECLS

#endif

