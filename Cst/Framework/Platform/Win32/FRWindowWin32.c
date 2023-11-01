#include <Framework/Device/FRWindow.h>


HWND fr_window_get_win32_window(FRWindow* self) {
  sys_return_val_if_fail(self != NULL, NULL);
  GLFWwindow *gwindow = fr_window_get_gwindow(self);

  return glfwGetWin32Window(gwindow);
}

