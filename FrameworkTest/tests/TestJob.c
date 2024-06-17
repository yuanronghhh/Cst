#include <FrameworkTest/tests/TestJob.h>

static SysPointer task_exec(FrJobTask *task, SysPointer user_data) {
  sys_debug_N("%p, %d", task, user_data);
  sys_usleep(3e6);

  return NULL;
}

static SysPointer task_exec2(FrJobTask *task, SysPointer user_data) {
  sys_debug_N("%p, %d", task, user_data);
  sys_usleep(1e6);

  return NULL;
}

static SysPointer task_done(FrJobTask *task, SysPointer user_data) {
  sys_debug_N("task callback: %p, %d", task, user_data);

  return NULL;
}

static SysPointer job_thread(FrJob *job, SysPointer user_data) {
  return NULL;
}

void test_job_basic(void) {
  FrJob *job;
  FrJobTask *task;
  FrJobTaskContext task_info = {0};
  FrJobContext job_ctx = {0};
  SysInt i = 0;

  SysHArray *tasklist = sys_harray_new_with_free_func((SysDestroyFunc)_sys_object_unref);

  job_ctx.name = "test job";
  job_ctx.callback = job_thread;
  job_ctx.user_data = "abc";
  job = fr_job_new_I(&job_ctx);

  task_info.handler = task_exec;
  task_info.callback = task_done;

  fr_job_start(job);

  while(i < 2) {
    task_info.user_data = INT_TO_POINTER(i);
    task_info.handler = i == 0 ? task_exec : task_exec2;

    task = fr_job_task_new_I(&task_info);
    sys_harray_add(tasklist, task);
    sys_debug_N("send: %p", task);
    fr_job_send_task_wait(job, task);

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
