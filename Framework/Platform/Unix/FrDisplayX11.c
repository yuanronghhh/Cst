#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrIWindow.h>


Display* fr_display_get_x11_display(FrDisplay *display) {

  return (Display  *)fr_i_window_get_native_display(display);
}
