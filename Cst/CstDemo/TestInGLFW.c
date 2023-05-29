#include <TestInGLFW.h>

typedef struct _Driver Driver;

struct _Driver {
  GLFWwindow       *window;
  cairo_t          *cairo;
  cairo_surface_t  *surface;

  int height;
  int width;

#ifdef SYS_OS_UNIX
  Window  nwindow;
#elif SYS_OS_WIN32
  HWND nwindow;
#endif
};

static void error_callback(int e, const char *d) {
  printf("Error %d: %s\n", e, d);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  printf("[Key]%d\t%d\t%d\t%d\n", key, scancode, action, mods);
}

Driver *get_driver_context() {
  GLFWwindow *window;
  int width = 1200, height = 800;
  int fbw, fbh;
  Driver *dv = g_new0(Driver, 1);

  /* GLFW */
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    fprintf(stdout, "[GFLW] failed to init!\n");
    glfwTerminate();
    exit(1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, "Demo", NULL, NULL);

  glfwMakeContextCurrent(window);

  glfwGetFramebufferSize(window, &fbw, &fbh);

  dv->width = fbw;
  dv->height = fbh;

#ifdef SYS_OS_UNIX
  dv->nwindow = glfwGetX11Window(window);
  dv->surface = cairo_xlib_surface_create(glfwGetX11Display(), glfwGetX11Window(window), DefaultVisual(dv->ndisplay, DefaultScreen(dv->ndisplay)), fbw, fbh);
#else
  dv->nwindow = glfwGetWin32Window(window);
  dv->surface = cairo_win32_surface_create(GetDC(dv->nwindow));
#endif

  dv->cairo = cairo_create(dv->surface);
  dv->window = window;

  return dv;
}

void destroy_driver_context(Driver *dv) {
  cairo_destroy(dv->cairo);
  cairo_surface_destroy(dv->surface);
  glfwDestroyWindow(dv->window);
  glfwTerminate();
}

static void draw_hello(cairo_t *cr) {
  static int i = 0;
  if (i < 50) {
    i += 1;
  }
  cairo_save(cr);
  cairo_set_font_size (cr, 20);
  cairo_move_to (cr, 20 + i, 20 + i);
  cairo_show_text (cr, "Hello World.");
  cairo_restore(cr);
}

static void draw_link(cairo_t *cr) {
  cairo_tag_begin (cr, CAIRO_TAG_LINK, "uri='https://cairographics.org'");
  cairo_move_to (cr, 50, 50);
  cairo_show_text (cr, "This is a link to the cairo website.");
  cairo_tag_end (cr, CAIRO_TAG_LINK);
}

static void gui_draw(Driver *dv) {
  cairo_t *cr = dv->cairo;
  cairo_surface_t *surface = dv->surface;
  cairo_status_t status;
  cairo_region_t *region = cairo_region_create();
  const char *e;

  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

  draw_hello(cr);

  status = cairo_status(cr);
  if(status != CAIRO_STATUS_SUCCESS) {
    e = cairo_status_to_string(status);
    printf("%s\n", e);
  }

  cairo_region_destroy (region);
}

static void test_glfw_window(void) {
  Driver *dv;
  int fbw, fbh;

  dv = get_driver_context();
  glfwGetFramebufferSize(dv->window, &fbw, &fbh);

  while(!glfwWindowShouldClose(dv->window)) {
    glfwSetKeyCallback(dv->window, key_callback);

    /* Draw */
    glViewport(0, 0, fbw, fbh);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    gui_draw(dv);

    glfwSwapBuffers(dv->window);
    glfwPollEvents();
  }

  destroy_driver_context(dv);
}

void test_glfw_init(void) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_glfw_window);
  }
  UNITY_END();
}
