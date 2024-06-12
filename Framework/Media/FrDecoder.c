#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrIDecoder.h>

SYS_DEFINE_TYPE(FrDecoder, fr_decoder, SYS_TYPE_OBJECT);

#define DECODER_LOCK sys_mutex_lock(&self->ctrl.mutex)
#define DECODER_UNLOCK sys_mutex_unlock(&self->ctrl.mutex)

void fr_decoder_lock(FrDecoder* self) {
  DECODER_LOCK;
}

void fr_decoder_unlock(FrDecoder* self) {
  DECODER_UNLOCK;
}

const SysChar* fr_decoder_get_name(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

SysBool fr_decoder_enough_unlock(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, false);
  SysInt r;
  SysUInt len;
  FrDecoder *o = FR_DECODER(self);

  len = fr_decoder_get_length(o);
  r = len >= self->max_pkt;

  return r;
}

SysBool fr_decoder_not_enough_unlock(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, false);
  FrDecoder *o = FR_DECODER(self);
  SysUInt len = fr_decoder_get_length(o);

  return len <= self->min_pkt;
}

SysBool fr_decoder_get_length(FrDecoder *self) {

  return sys_queue_get_length(&self->ctrl.queue);
}

static SysInt fr_decoder_decode_it_i(FrDecoder *self, FrPacket *pkt) {
  sys_return_val_if_fail(self != NULL, -1);

  return FR_MEDIA_ERROR_WAIT;
}

void fr_decoder_set_task(FrDecoder* self, FrMediaTask* task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  DECODER_LOCK;
  sys_assert(self->task == NULL);

  self->task = task;
  self->state = FR_MEDIA_STATE_RUNNING;
  sys_cond_signal(&self->ctrl.cond);

  DECODER_UNLOCK;
}

static SysInt decoder_process_task(FrDecoder* self) {
  SysInt result = 0;
  if(!self->task) { return result; }

  fr_media_task_run(self->task);
  result = POINTER_TO_INT(fr_media_task_result(self->task));
  self->task = NULL;

  return result;
}

static SysInt decoder_process_packet(FrDecoder* self) {
  SysInt err;
  FrPacket *npkt = NULL;

  err = fr_decoder_decode_check(self);
  if(!fr_decoder_pop_packet_unlock(self, &npkt)) {

    return FR_MEDIA_ERROR_WAIT;
  }
  err = fr_decoder_decode_it(self, npkt);

  return err;
}

static SysInt error_to_state(SysInt err) {
  switch (err) {
  case FR_MEDIA_ERROR_AGAIN:
    return FR_MEDIA_STATE_RUNNING;
    break;

  case FR_MEDIA_ERROR_EXIT:
    return FR_MEDIA_STATE_STOP;
  case FR_MEDIA_ERROR_WAIT:
  case FR_MEDIA_ERROR_EOF:
    return FR_MEDIA_STATE_PAUSE;
    break;
  default:
    return FR_MEDIA_STATE_PAUSE;
  }
}

static FR_MEDIA_STATE_ENUM decoder_process(FrDecoder* self) {
  SysInt err = 0;
  FR_MEDIA_STATE_ENUM state;
  DECODER_LOCK;

  err = decoder_process_task(self);
  if (err < 0) { goto done; }

  err = decoder_process_packet(self);

done:
  state = error_to_state(err);
  self->state = state;
  DECODER_UNLOCK;

  if (state < 0) {
    sys_warning_N("process packet failed: %s,%s", 
      self->name,
      fr_media_error_string(err));
  }

  return state;
}

static SysPointer decoder_thread(SysPointer user_data) {
  FrDecoder *self = user_data;
  FR_MEDIA_STATE_ENUM state = 0;

  while (self->state == FR_MEDIA_STATE_RUNNING) {
    state = decoder_process(self);

    switch (state) {
      case FR_MEDIA_STATE_PAUSE:
        fr_decoder_wait(self);
        break;
      case FR_MEDIA_STATE_RUNNING:
        break;
      case FR_MEDIA_STATE_STOP:
        goto exit;
    }
  }

exit:
  return NULL;
}

void fr_decoder_wakeup_unlock(FrDecoder* self) {
  sys_return_if_fail(self != NULL);

  self->state = FR_MEDIA_STATE_RUNNING;
  sys_cond_signal(&self->ctrl.cond);
}

void fr_decoder_wait (FrDecoder* self) {
  DECODER_LOCK;
  while(self->state == FR_MEDIA_STATE_PAUSE) {

    sys_cond_wait(&self->ctrl.cond, &self->ctrl.mutex);
  }
  DECODER_UNLOCK;
}

static SysPointer init_it(FrMediaTask* task, SysPointer user_data) {
  FrDecoder *self = user_data;

  sys_debug_N("init it: %s", self->name);
  return NULL;
}

void decoder_wait_init(FrDecoder* self) {
  FrMediaTask task = { 0 };
  task.handler = init_it;
  task.data = self;

  fr_decoder_set_task(self, &task);
  fr_media_task_wait(&task);
}

SysInt fr_decoder_start(FrDecoder* self) {
  SysThread *thread;
  const SysChar *name = self->name;

  thread = sys_thread_new(name, decoder_thread, self);
  if (thread == NULL) {
    sys_error_N("decoder start failed: \"%s\"", name);
    return -1;
  }
  self->thread = thread;
  decoder_wait_init(self);

  return 0;
}

static SysPointer stop_it(FrMediaTask *task, SysPointer user_data) {
  FrDecoder *self = user_data;

  self->state = FR_MEDIA_STATE_STOP;

  return INT_TO_POINTER(FR_MEDIA_ERROR_EXIT);
}

void fr_decoder_stop(FrDecoder* self) {
  sys_return_if_fail(self != NULL);
  if (fr_decoder_get_state(self) == FR_MEDIA_STATE_STOP) { return; }

  FrMediaTask task = {0};
  task.handler = stop_it;
  task.data = self;

  fr_decoder_set_task(self, &task);
  fr_media_task_wait(&task);
  sys_thread_join(self->thread);
}

void fr_decoder_set_state(FrDecoder *self, FR_MEDIA_STATE_ENUM state) {
  sys_return_if_fail(self != NULL);

  DECODER_LOCK;
  self->state = state;
  DECODER_UNLOCK;
}

FR_MEDIA_STATE_ENUM fr_decoder_get_state(FrDecoder *self) {
  sys_return_val_if_fail(self != NULL, false);
  FR_MEDIA_STATE_ENUM r;

  DECODER_LOCK;
  r = self->state;
  DECODER_UNLOCK;

  return r;
}

void fr_decoder_set_user_data(FrDecoder *self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  self->user_data = user_data;
}

SysPointer fr_decoder_get_user_data(FrDecoder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->user_data;
}

SysBool fr_decoder_pop_packet_unlock(FrDecoder* self,
    FrPacket **pkt) {
  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(*pkt == NULL, -1);
  FrPacket *npkt;

  if(!sys_queue_get_length(&self->ctrl.queue)) {
    return false;
  }

  npkt = sys_queue_pop_tail(&self->ctrl.queue);
  if(fr_packet_empty(npkt)) {
    sys_object_unref(npkt);

    return false;
  }
  self->serial++;;
  *pkt = npkt;

  return true;
}

SysBool fr_decoder_pop_packet(FrDecoder* self,
    FrPacket** pkt) {
    SysBool r;

    DECODER_LOCK;
    r = fr_decoder_pop_packet_unlock(self, pkt);
    DECODER_UNLOCK;

    return r;
}

SysBool fr_decoder_push_packet(FrDecoder* self, FrPacket* pkt) {
  SysBool r;

  DECODER_LOCK;
  r = fr_decoder_push_packet_unlock(self, pkt);
  fr_decoder_wakeup_unlock(self);
  DECODER_UNLOCK;

  return r;
}

SysBool fr_decoder_push_packet_unlock(FrDecoder* self, FrPacket *pkt) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(pkt != NULL, false);

  sys_queue_push_head (&self->ctrl.queue, pkt);
  fr_decoder_wakeup_unlock(self);

  return true;
}

SysInt fr_decoder_open(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, -1);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->open, -1);

  return cls->open(self);
}

SysInt fr_decoder_decode_check(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, -1);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->decode_check, -1);

  return cls->decode_check(self);
}

SysInt fr_decoder_decode_check_i(FrDecoder* self) {

  return FR_MEDIA_ERROR_WAIT;
}

SysInt fr_decoder_decode_it(FrDecoder* self, FrPacket *npkt) {
  sys_return_val_if_fail(self != NULL, -1);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->decode_it, -1);

  return cls->decode_it(self, npkt);
}

SysInt fr_decoder_close(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, -1);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->close, -1);

  return cls->close(self);
}

void fr_decoder_construct(FrDecoder* self,
    const SysChar *name) {
  sys_return_if_fail(self != NULL);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);

  sys_return_if_fail(cls->construct);

  cls->construct(self, name);
}

SysInt fr_decoder_open_i(FrDecoder* o) {
  return 0;
}

SysInt fr_decoder_close_i(FrDecoder* o) {
  return 0;
}

/* object api */
static void fr_decoder_construct_i(FrDecoder *self,
    const SysChar *name) {
  self->name = sys_strdup(name);
}

FrDecoder* fr_decoder_new(void) {
  return sys_object_new(FR_TYPE_DECODER, NULL);
}

static void fr_decoder_dispose(SysObject* o) {
  FrDecoder *self = FR_DECODER(o);

  fr_decoder_stop(self);

  sys_cond_clear(&self->ctrl.cond);
  sys_mutex_clear(&self->ctrl.mutex);
  sys_queue_clear(&self->ctrl.queue);
  sys_free_N(self->name);


  SYS_OBJECT_CLASS(fr_decoder_parent_class)->dispose(o);
}

static void fr_decoder_class_init(FrDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_decoder_construct_i;
  cls->open = fr_decoder_open_i;
  cls->close = fr_decoder_close_i;
  cls->decode_it = fr_decoder_decode_it_i;
  cls->decode_check = fr_decoder_decode_check_i;

  ocls->dispose = fr_decoder_dispose;
}

void fr_decoder_init(FrDecoder* self) {
  self->start_pts = -1;
  self->serial = -1;
  self->state = FR_MEDIA_STATE_RUNNING;
  self->task = NULL;
  self->min_pkt = 24;
  self->max_pkt = 96;

  sys_queue_init(&self->ctrl.queue);
}
