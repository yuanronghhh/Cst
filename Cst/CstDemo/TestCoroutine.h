#ifndef __TEST_COROUTINE_H__
#define __TEST_COROUTINE_H__

#include <SysCore.h>
#include <stdbool.h>
#include <glib.h>
#include <stdio.h>
#include <gio/gio.h>
#include <gstdio.h>
#include "gcoroutine.h"

G_BEGIN_DECLS

int test_coroutine_init(int argc, char **argv);

G_END_DECLS

#endif
