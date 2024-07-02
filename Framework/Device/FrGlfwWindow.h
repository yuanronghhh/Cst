#ifndef __FR_GLFW_WINDOW_H__
#define __FR_GLFW_WINDOW_H__

#include <Framework/Device/FrWindow.h>

SYS_BEGIN_DECLS

#define FR_TYPE_GLFW_WINDOW (fr_glfw_window_get_type())
#define FR_GLFW_WINDOW(o) ((FrGlfwWindow* )sys_object_cast_check(o, FR_TYPE_GLFW_WINDOW))
#define FR_GLFW_WINDOW_CLASS(o) ((FrGlfwWindowClass *)sys_class_cast_check(o, FR_TYPE_GLFW_WINDOW))
#define FR_GLFW_WINDOW_GET_CLASS(o) sys_instance_get_class(o, FrGlfwWindowClass)

struct _FrGlfwWindowClass {
  FrWindowClass parent;
};

struct _FrGlfwWindow {
  FrWindow parent;
  /* <private> */
  GLFWwindow *gwindow;
  SysPointer native_window;
};

SYS_API SysType fr_glfw_window_get_type(void);

void fr_glfw_window_setup(void);
void fr_glfw_window_iface_setup(FrIWindowInterface *iface);
void fr_glfw_window_teardown(void);

SYS_END_DECLS

#endif

