#include <Framework/Device/FRWindowPrivate.h>


HWND fr_window_get_win32_window(FRWindow* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRWindowPrivate* priv = self->priv;

  return glfwGetWin32Window(priv->gwindow);
}

