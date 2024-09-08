#include <Framework/Media/FrPlayer.h>
#include <Framework/Device/FrWindow.h>

SYS_DEFINE_TYPE(FrPlayer, fr_player, SYS_TYPE_OBJECT);

SysInt fr_player_run(FrPlayer* self) {
  sys_return_val_if_fail(self != NULL, -1);
  
  FrPlayerClass *cls;
  SysInt err;
  SysBool first = true;

  if(self->state != -1) { return -1; }

  cls = FR_PLAYER_GET_CLASS(self);

  err = cls->init(self);
  if(err < 0) { return err; }

  while(self->state == FR_JOB_STATE_RUNNING) {
    self->loop_count += 1;

    if(first) {

      cls->started(self);
      first = false;
    }

    cls->process(self);
  }

  cls->stop(self);

  return 0;
}

void fr_player_set_state(FrPlayer *self, FR_JOB_STATE_ENUM state) {
  sys_return_if_fail(self != NULL);

  self->state = state;
}

FR_JOB_STATE_ENUM fr_player_get_state(FrPlayer *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->state;
}

static SysInt fr_player_stop_i(FrPlayer *self) {

  return 0;
}

void fr_player_set_base_delay(FrPlayer *self, SysUInt64 delay_ms) {

  self->base_delay = delay_ms;
}

static SysInt fr_player_process_i(FrPlayer *self) {
  if(self->base_delay == -1) {

    sys_error_N("%d", "Must call fr_player_set_base_delay before run player");
  }

  SysUInt64 current = sys_get_monotonic_time();
  SysInt64 diff = current - self->frame_clock.last;
  SysInt64 ndiff = self->base_delay - diff;
  SysInt64 delay;

  if(ndiff > 0) {

    delay = ndiff;
  } else {

    delay = 0;
  }
  self->frame_clock.last = current;

  fr_poll_events();
  fr_delay(delay);

  return 0;
}

static SysInt fr_player_init_i(FrPlayer *self) {
  self->frame_clock.start = sys_get_monotonic_time();
  self->frame_clock.last = -1;
  self->frame_clock.frame_count = 0;

  return 0;
}

static SysInt fr_player_started_i(FrPlayer *self) {

  return 0;
}

/* object api */
static void fr_player_construct_i(FrPlayer *self) {
}

FrPlayer* fr_player_new(void) {

  return sys_object_new(FR_TYPE_PLAYER, NULL);
}

FrPlayer *fr_player_new_I(void) {
  FrPlayer *o = fr_player_new();

  fr_player_construct_i(o);

  return o;
}

static void fr_player_dispose(SysObject* o) {
  FrPlayer *self = FR_PLAYER(o);

  self->state = -1;

  SYS_OBJECT_CLASS(fr_player_parent_class)->dispose(o);
}

static void fr_player_class_init(FrPlayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->started = fr_player_started_i;
  cls->init = fr_player_init_i;
  cls->process = fr_player_process_i;
  cls->stop = fr_player_stop_i;

  ocls->dispose = fr_player_dispose;
}

void fr_player_init(FrPlayer* self) {
  self->state = -1;
  self->loop_count = 0;
  self->base_delay = -1;
}
