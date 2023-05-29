#ifndef __CST_DEMO_H__
#define __CST_DEMO_H__

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <fcntl.h>
#include "glib.h"

G_BEGIN_DECLS

// #define CST_TYPE_DEFINE_CAST (GtkWindow, GTK_WINDOW, GTK_TYPE_WINDOW, GtkWidget, GTK_TYPE_WIDGET)

#define CST_TYPE_DEFINE_CAST_CODE(TN, T_N, T_T_N, TP, T_T_P)              \
(TN *) (T_N)(T_P *obj)                                                    \
{                                                                         \
  return (G_TYPE_CHECK_INSTANCE_CAST ((obj), T_T_N, TN));                 \
}                                                                         \
                                                                          \
(TN##Class *) (T_N##_CLASS)(TN##Class *kclass)                            \
{                                                                         \
  return (G_TYPE_CHECK_CLASS_CAST ((klass), T, TN##Class));               \
}                                                                         \
                                                                          \
gboolean (IS_##T_N)(T_P *obj)                                             \
{                                                                         \
  return (G_TYPE_CHECK_INSTANCE_TYPE ((obj), T_T_N));                     \
}                                                                         \
                                                                          \
gboolean (IS_##T_N##_CLASS)(TP##Class *obj)                               \
{                                                                         \
  return (G_TYPE_CHECK_INSTANCE_TYPE ((obj), T_T_N));                     \
}                                                                         \
                                                                          \
(TN##Class *) (T_N##_GET_CLASS)(TP##Class *obj)                           \
{                                                                         \
  return (G_TYPE_INSTANCE_GET_CLASS ((obj), T_T_N, TN##Class));           \
}

G_END_DECLS
#endif // END __CST_DEMO_H__
