#ifndef __TEST_MONO__
#define __TEST_MONO__

#include "DemoCommon.h"
#include <mono/jit/jit.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/class.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-gc.h>

SYS_BEGIN_DECLS

void test_mono_init(int argc, char *argv[]);

SYS_END_DECLS

#endif
