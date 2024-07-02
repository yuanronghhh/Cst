#ifndef __FR_I_WINDOW__
#define __FR_I_WINDOW__

#include <Framework/FrCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_WINDOW (fr_i_window_get_type())
#define FR_I_WINDOW(o) ((FrIWindow* )sys_object_cast_check(o, FR_TYPE_I_WINDOW))
#define FR_I_WINDOW_GET_IFACE(o) ((FrIWindowInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_WINDOW))

struct _FrIWindowInterface {
  SysTypeInterface parent;

  /* <private> */
  void (*create) (
      FrWindow* window,
      SysInt width,
      SysInt height,
      const SysChar *title,
      FrWindow *share);
  void (*set_error_callback) (FrWindowErrFunc callback);
  void (*get_framebuffer_size) (FrWindow *window, SysInt *width, SysInt *height);
  void (*window_get_size) (FrWindow *window, SysInt *width, SysInt *height);
  void (*window_set_size) (FrWindow *window, SysInt width, SysInt height);
  void (*window_set_title) (FrWindow *window, const SysChar *title);
  void (*window_set_opacity) (FrWindow *window, SysDouble opacity);
  void (*window_set_should_close) (FrWindow *window, SysBool bvalue);
  void (*swap_buffers) (FrWindow *window);
  void (*window_destroy) (FrWindow *window);
  const SysChar* (*get_key_name) (SysInt key, SysInt scancode);
  SysInt (*get_key) (FrWindow *window, SysInt key);
  void (*wait_events_timeout) (SysDouble sec);
  void (*wait_events) (void);
  void (*post_empty_event) (void);
  void (*poll_events) (void);

  void (*display_create) (FrDisplay *display);
  SysPointer (*get_native_window) (FrWindow* window);
  SysPointer (*get_native_display) (FrDisplay *display);
};

SysType fr_i_window_get_type(void);
void fr_i_window_setup(void);
void fr_i_window_teardown(void);

void fr_i_window_create (
    FrWindow* window,
    SysInt width,
    SysInt height,
    const SysChar *title,
    FrWindow *share);

void fr_i_window_display_create (FrDisplay *display);
void fr_i_window_destroy_window (FrWindow *window);
void fr_i_window_set_error_callback (FrWindowErrFunc callback);
void fr_i_window_get_framebuffer_size (FrWindow *window, SysInt *width, SysInt *height);
void fr_i_window_window_get_size (FrWindow *window, SysInt *width, SysInt *height);
void fr_i_window_window_set_size (FrWindow *window, SysInt width, SysInt height);
void fr_i_window_window_set_title (FrWindow *window, const SysChar *title);
void fr_i_window_window_set_opacity (FrWindow *window, SysDouble opacity);
void fr_i_window_window_set_should_close (FrWindow *window, SysBool bvalue);
void fr_i_window_window_destroy (FrWindow *window);
void fr_i_window_swap_buffers (FrWindow *window);
const SysChar *fr_i_window_get_key_name (SysInt key, SysInt scancode);
SysInt fr_i_window_get_key (FrWindow *window, SysInt key);
void fr_i_window_wait_events_timeout (SysDouble sec);
void fr_i_window_wait_events (void);
void fr_i_window_post_empty_event (void);
void fr_i_window_poll_events (void);
SysPointer fr_i_window_get_native_window (FrWindow* window);
SysPointer fr_i_window_get_native_display (FrDisplay *display);

SYS_END_DECLS

#endif
