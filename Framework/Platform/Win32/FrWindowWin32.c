#include <Framework/Device/FrWindow.h>


HWND fr_window_get_win32_window(FrWindow* self) {
  sys_return_val_if_fail(self != NULL, NULL);
  GLFWwindow *gwindow = fr_window_get_gwindow(self);

  return glfwGetWin32Window(gwindow);
}

