#include <FrameworkTest/TestCore.h>
#include <FrameworkTest/tests/TestVideo.h>
#include <FrameworkTest/tests/TestJob.h>

#define CHECK_TYPE SYS_TYPE_OBJECT

void setUp(void) {
}

void tearDown(void) {
}
static SysInt i = 0;

static void object_new_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, CHECK_TYPE)) {
    return;
  }

  sys_debug_N("%d\t%p\t%s\t%d", ++i, o, name, ref_count);
  sys_assert(ref_count == 1);
}

static void object_ref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, CHECK_TYPE)) {
    return;
  }

  sys_debug_N("%p\t%s\t%d", o, name, ref_count);
  sys_assert(ref_count > 0);
}

static void object_unref_debug(SysObject *o, const SysChar *name, SysInt ref_count) {
  if (!sys_object_is_a(o, CHECK_TYPE)) {
    return;
  }

  sys_debug_N("%p\t%s\t%d", o, name, ref_count);
  sys_assert(ref_count > 0);
}

int main(int argc, const SysChar* argv[]) {
  sys_set_debugger(true);
  sys_setup();

#if 1
  sys_object_set_ref_hook(object_ref_debug);
  sys_object_set_unref_hook(object_unref_debug);
  sys_object_set_new_hook(object_new_debug);
#endif
  fr_core_setup();

  test_video_init(argc, argv);

  fr_core_teardown();
  sys_teardown();
  return 0;
}
