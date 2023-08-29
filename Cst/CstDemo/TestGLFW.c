#include <CstDemo/TestGLFW.h>

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};
 
static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";
 
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

cairo_surface_t* glfw_create_surface(GLFWwindow *gwindow, SysInt width, SysInt height) {
  sys_return_val_if_fail(gwindow != NULL, NULL);

  cairo_surface_t *surface;

#if SYS_OS_WIN32
  HWND hwd = glfwGetWin32Window(gwindow);
  HDC hdc = GetDC(hwd);
  surface = cairo_win32_surface_create_with_format(hdc, CAIRO_FORMAT_ARGB32);
#elif SYS_OS_UNIX
  Window xwindow = glfwGetX11Window(gwindow);
  Display *ndisplay = glfwGetX11Display();
  int nscreen = DefaultScreen(ndisplay);
  Visual *nvisual = DefaultVisual(ndisplay, nscreen);

  surface = cairo_xlib_surface_create(ndisplay,
      xwindow,
      nvisual,
      width, height);
#endif

  return surface;
}

void test_glfw_basic(void) {
  GLFWwindow* window;
  GLuint vertex_buffer, vertex_shader, fragment_shader, program;
  GLint vpos_location, vcol_location;
  // GLint mvp_location;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    sys_error_N("%s", "glfw init error");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  // NOTE: OpenGL error checks have been omitted for brevity

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
  glCompileShader(fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  // mvp_location = glGetUniformLocation(program, "MVP");
  vpos_location = glGetAttribLocation(program, "vPos");
  vcol_location = glGetAttribLocation(program, "vCol");

  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
      sizeof(vertices[0]), (void*) 0);
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
      sizeof(vertices[0]), (void*) (sizeof(float) * 2));

  while (!glfwWindowShouldClose(window))
  {
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    glfwPollEvents();

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwWaitEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

#if SYS_OS_UNIX
static void test_cairo_x11_transparent(void) {
  Display *display;
  Window window;
  XVisualInfo vinfo;
  XSetWindowAttributes attr;
  cairo_surface_t *surface;
  cairo_t *cr;
  int screen;

  // 连接X服务器
  display = XOpenDisplay(NULL);
  if (!display) {
    return;
  }

  // 获取屏幕信息
  screen = DefaultScreen(display);

  // 获取可用的Visual信息
  XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo);

  // 设置窗口属性
  attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
  attr.background_pixel = -2;
  attr.border_pixel = 0;
  attr.override_redirect = True;

  // 创建窗口
  window = XCreateWindow(display, DefaultRootWindow(display),
      0,
      0,
      640,
      480,
      0, 
      vinfo.depth,
      InputOutput,
      vinfo.visual,
      CWColormap | CWBackPixel | CWBorderPixel | CWOverrideRedirect,
      &attr);

  // 绘制窗口内容
  surface = cairo_xlib_surface_create(display, window, vinfo.visual, 640, 480);
  cr = cairo_create(surface);

  cairo_set_source_rgba(cr, 0, 0, 0, 0);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);
  cairo_destroy(cr);

  // 显示窗口
  XMapWindow(display, window);
  XFlush(display);

  // 进入事件循环
  while (1) {
    XEvent event;
    XNextEvent(display, &event);
  }

  // 销毁窗口和Cairo绘图上下文
  cairo_surface_destroy(surface);
  XDestroyWindow(display, window);

  // 断开与X服务器的连接
  XCloseDisplay(display);
}
#endif

static void test_glfw_vulkan(void) {
  GLFWwindow* window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    sys_error_N("%s", "glfw init error");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

  if (!glfwVulkanSupported()) {
    sys_error_N("%s", "glfwVulkanSupported return false.");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(640, 480, "vulkan example", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  while (!glfwWindowShouldClose(window))
  {
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    glfwPollEvents();

    glfwSwapBuffers(window);
    glfwWaitEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

static void test_cairo_transparent(void) {
  GLFWwindow* window;
  int width, height;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    sys_error_N("%s", "glfw init error");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

  width = 640;
  height = 480;

  window = glfwCreateWindow(width, height, "cairo transparent example", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  while (!glfwWindowShouldClose(window)) {
    glfwGetFramebufferSize(window, &width, &height);

    cairo_surface_t *surface = glfw_create_surface(window, width, height);

    cairo_t* cr = cairo_create(surface);

    // printf("%d,%d\n", CAIRO_OPERATOR_OVER, cairo_get_operator(cr));

    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);

    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 0.1);

    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    cairo_paint(cr);
    cairo_destroy(cr);

    cairo_surface_flush(surface);
    cairo_surface_destroy(surface);

    glfwWaitEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
}

void test_glfw_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    // RUN_TEST(test_glfw_vulkan);
    // RUN_TEST(test_glfw_basic);
    RUN_TEST(test_cairo_transparent);
  }
  UNITY_END();
}
