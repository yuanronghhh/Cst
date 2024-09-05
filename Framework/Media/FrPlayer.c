#include <Framework/Media/FrPlayer.h>

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


static SysInt fr_player_process_i(FrPlayer *self) {

  return 0;
}

static SysInt fr_player_init_i(FrPlayer *self) {

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
}
