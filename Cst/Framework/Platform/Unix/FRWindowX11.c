#include <Framework/Device/FRWindowPrivate.h>


Window fr_window_get_x11_window(FRWindow* self) {
  sys_return_val_if_fail(self != NULL, -1);

  FRWindowPrivate* priv = self->priv;

  return glfwGetX11Window(priv->gwindow);
}
