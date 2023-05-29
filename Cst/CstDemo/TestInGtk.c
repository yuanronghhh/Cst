#include <TestInGtk.h>
#include "pixman.h"

bool test_draw_callback(
    GtkWidget *widget,
    cairo_t *cr,
    gpointer data) {

  printf("[test_draw_callback called]\n");
  GdkRGBA color = { 1, 1, 1, 1 };

  int width, height;
  int i;

  cairo_surface_t *surface = cairo_get_target(cr);

  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);

  cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);

  cairo_save (cr);
  cairo_set_font_size (cr, 40);
  cairo_move_to (cr, 40, 60);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_show_text (cr, "Hello World");
  cairo_restore (cr);

  cairo_set_source_rgb (cr, 1,0,0);
  cairo_set_font_size (cr, 20);
  cairo_move_to (cr, 50, 100);
  cairo_show_text (cr, "greetings from gtk and cairo");

  gdk_cairo_set_source_rgba (cr, &color);

  cairo_move_to (cr, 0, 150);
  for (i = 0; i < width / 10; i++) {
    cairo_rel_line_to (cr, 5,  10);
    cairo_rel_line_to (cr, 5, -10);
  }
  cairo_stroke (cr);

  return true;
}

static void test_clicked(GtkWidget *widget, gpointer user_data) {
  printf("clicked\n");
}

void test_gtk_app_start(GtkApplication* app, gpointer user_data) {

  GtkWidget *window;
  GtkWidget *canvas;
  GtkWidget *btn;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "CairoDemo");

  canvas = gtk_drawing_area_new ();

  btn = gtk_button_new_with_label("demo");

  gtk_widget_set_size_request (canvas, 400, 200);

  gtk_window_set_child(GTK_WINDOW(window), btn);

  gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

  gtk_window_present(GTK_WINDOW(window));
}

gboolean on_window_map(GtkWidget * da) {
  static int oldw = 0;
  static int oldh = 0;
  return TRUE;
}

static void motion_cb(GtkEventControllerMotion *controller,
  double                    x,
  double                    y,
  GtkWindow             *self)
{
  g_debug("%d,%d", x, y);
}

void cairo_animation_active(GtkApplication* app, gpointer user_data) {
  GtkWidget *window_widget;
  GtkWidget *canvas;
  GtkWindow *window;
  GtkWidget *btn;
  int width = 400;
  int height = 200;

  btn = gtk_button_new();

  window_widget = gtk_application_window_new(app);
  window = GTK_WINDOW(window_widget);

  gtk_window_set_title(window, "CairoDemo");

  g_signal_connect(G_OBJECT(window), "map", G_CALLBACK(on_window_map), NULL);

  GtkEventController *ec = gtk_event_controller_motion_new();
  g_signal_connect(ec, "motion", G_CALLBACK(motion_cb), window);
  gtk_widget_add_controller(window_widget, ec);

  canvas = gtk_drawing_area_new();
  gtk_widget_set_size_request(canvas, width, height);
  gtk_window_set_default_size(window, width, height);

  gtk_window_set_child(window, canvas);

  gtk_window_present(window);
}

void test_cairo_animation(void) {
  GtkApplication *app;

  app = gtk_application_new("org.cst.demo", G_APPLICATION_FLAGS_NONE);

  g_signal_connect(app, "activate", G_CALLBACK(cairo_animation_active), NULL);
  g_application_run(G_APPLICATION(app), 0, NULL);
  g_object_unref(app);
}


void test_gtk_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_cairo_animation);
  }
  UNITY_END();
}
