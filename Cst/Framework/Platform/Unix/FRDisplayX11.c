#include <Framework/Device/FRDisplay.h>


Display* fr_display_get_x11_display(FRDisplay *display) {
  Display *xdisplay = glfwGetX11Display();

  return xdisplay;
}
