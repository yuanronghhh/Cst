#ifndef __TEST_LLVM__
#define __TEST_LLVM__

#include <stdbool.h>
#include "llvm-c/Analysis.h"
#include "clang-c/Index.h"
#include "llvm-c/Core.h"
#include "SysCore.h"
#include "glib.h"
#include "unity.h"

G_BEGIN_DECLS

void test_llvm_init(int argc, char *argv[]);

G_END_DECLS

#endif
