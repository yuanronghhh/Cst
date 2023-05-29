#include <stdlib.h>
#include <stdbool.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include "glib.h"

typedef struct _CstEventSource CstEventSource;

static GMutex main_lock;
static GSList *main_loops = NULL;


typedef struct _CstEventSource {
  GSource source;
  GPollFD pfd;
  guint mask;
  Display *display;
  gboolean reading;
} GdkWaylandEventSource;

static gboolean cst_event_source_prepare (GSource *base, gint    *timeout);
static gboolean cst_event_source_check (GSource *base);
static gboolean cst_event_source_dispatch (GSource     *base,
    GSourceFunc  callback,
    gpointer     data);
static void cst_event_source_finalize (GSource *base);

static GSourceFuncs cst_source_funcs = {
  cst_event_source_prepare,
  cst_event_source_check,
  cst_event_source_dispatch,
  cst_event_source_finalize
};

static gboolean cst_event_source_prepare (GSource *base, gint    *timeout) {
  printf("prepare called.\n");

  *timeout = -1;

  return false;
}

static gboolean cst_event_source_check (GSource *base) {
  printf("2.check\n");
  return true;
}

static gboolean cst_event_source_dispatch (GSource     *base,
    GSourceFunc  callback,
    gpointer     data) {
  printf("3.dispatch\n");
  return true;
}

static void cst_event_source_finalize (GSource *base) {
  printf("finalized\n");
}

static bool event_called(gpointer data) {
  GMainLoop *loop = (GMainLoop *)data;

  printf("event\n");
  return true;
}

static CstEventSource* cst_display_x11_new_source() {
  GSource *source;
  CstEventSource *cSource;
  Display *display;

  source = g_source_new (&cst_source_funcs,
      sizeof (CstEventSource));
  g_source_set_name(source, "test.main.loop");

  cSource = (CstEventSource *)source;

  display = XOpenDisplay(NULL);
  cSource->display = display;
  cSource->pfd.fd = ConnectionNumber(display);
  cSource->pfd.events = G_IO_IN | G_IO_ERR | G_IO_HUP;
  g_source_add_poll (source, &cSource->pfd);

  g_source_set_priority (source, G_PRIORITY_DEFAULT);
  g_source_set_can_recurse (source, true);
  g_source_attach(source, NULL);

  return source;
}

void test_main_loop_init(void) {
  GMainLoop *loop = g_main_loop_new(NULL, true);
  CstEventSource *cSource;

  g_timeout_add(1000, (GSourceFunc)event_called, loop);

  cSource = cst_display_x11_new_source();

  main_loops = g_slist_prepend (main_loops, loop);
  if (g_main_loop_is_running(main_loops->data)) {
    g_main_loop_run(loop);
  }
}
