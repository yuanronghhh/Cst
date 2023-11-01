#include <Framework/Device/FRWindow.h>


Window fr_window_get_x11_window(FRWindow* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return glfwGetX11Window(self->gwindow);
}
