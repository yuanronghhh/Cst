#include <Framework/Device/FrWindow.h>


HWND fr_window_get_win32_window(FrWindow* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return (HWND)fr_i_window_get_native_window(self);
}

