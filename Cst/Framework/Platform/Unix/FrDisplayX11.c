#include <Framework/Device/FrDisplay.h>


Display* fr_display_get_x11_display(FrDisplay *display) {
  Display *xdisplay = glfwGetX11Display();

  return xdisplay;
}
