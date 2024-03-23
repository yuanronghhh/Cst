#include <Framework/Device/FrWindow.h>


Window fr_window_get_x11_window(FrWindow* self) {
  sys_return_val_if_fail(self != NULL, -1);
  GLFWwindow *gwindow = fr_window_get_gwindow(self);

  return glfwGetX11Window(gwindow);
}
