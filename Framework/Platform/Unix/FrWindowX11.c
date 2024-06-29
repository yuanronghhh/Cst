#include <Framework/Device/FrWindow.h>


Window fr_window_get_x11_window(FrWindow* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return POINTER_TO_UINT(fr_i_window_get_native_window(self));
}
