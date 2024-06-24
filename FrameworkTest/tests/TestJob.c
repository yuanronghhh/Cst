#include <FrameworkTest/tests/TestJob.h>

static SysPointer task_exec(FrTask *task, SysPointer user_data) {
  sys_debug_N("%p, %d", task, user_data);

  return NULL;
}

static SysPointer task_exec2(FrTask *task, SysPointer user_data) {
  sys_debug_N("%p, %d", task, user_data);
  sys_usleep(1e6);

  return NULL;
}

static SysPointer task_done(FrTask *task, SysPointer user_data) {
  sys_debug_N("task callback: %p, %d", task, user_data);

  return NULL;
}

static SysPointer job_thread(FrJob *job, SysPointer user_data) {
  return NULL;
}

void test_job_basic(void) {
  FrJob *job;
  FrTask *task;
  FrJobContext job_ctx = {0};
  SysInt i = 0;

  SysHArray *tasklist = sys_harray_new_with_free_func((SysDestroyFunc)_sys_object_unref);

  job_ctx.name = "test job";
  job_ctx.callback = job_thread;
  job_ctx.user_data = "abc";
  job = fr_job_new_I(&job_ctx);

  fr_job_start(job);

  while(i < 2) {
    task = fr_task_new_handler(task_exec, INT_TO_POINTER(i));
    fr_job_run_task_async(job, task);

    i++;
  }

  fr_job_stop(job);
  fr_job_join(job);
  sys_harray_unref(tasklist);
}

void test_job_init(int argc, const SysChar * argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_job_basic);
  }
  UNITY_END();
}
