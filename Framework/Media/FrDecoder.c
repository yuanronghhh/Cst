#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrIDecoder.h>

SYS_DEFINE_TYPE(FrDecoder, fr_decoder, SYS_TYPE_OBJECT);

#define DECODER_LOCK fr_job_lock(&self->job)
#define DECODER_UNLOCK fr_job_unlock(&self->job)

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

SysUInt fr_decoder_get_length(FrDecoder *self) {
  SysUInt len;

  DECODER_LOCK;

  len = sys_queue_get_length(&self->queue);

  DECODER_UNLOCK;

  return len;
}

static SysInt fr_decoder_decode_it_i(FrDecoder *self, FrPacket *pkt) {
  sys_return_val_if_fail(self != NULL, -1);

  return FR_MEDIA_ERROR_WAIT;
}

void fr_decoder_set_task(FrDecoder* self, FrJobTask* task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  fr_job_run_task(&self->job, self->task);
}

static SysInt decoder_process_task(FrDecoder* self) {
  SysInt result = 0;
  if(!self->task) { return result; }

  fr_job_task_run(self->task);
  result = POINTER_TO_INT(fr_job_task_result(self->task));
  self->task = NULL;

  return result;
}

static SysInt decoder_process_packet(FrDecoder* self) {
  SysInt err;
  FrPacket *npkt = NULL;

  err = fr_decoder_decode_check(self);
  if(err < 0) { return err; }

  if(!fr_decoder_pop_packet_unlock(self, &npkt)) {

    return FR_MEDIA_ERROR_WAIT;
  }
  err = fr_decoder_decode_it(self, npkt);

  return err;
}

static SysInt error_to_state(SysInt err) {
  switch (err) {
  case FR_MEDIA_ERROR_AGAIN:
    return FR_JOB_STATE_RUNNING;
    break;

  case FR_MEDIA_ERROR_EXIT:
    return FR_JOB_STATE_STOP;
  case FR_MEDIA_ERROR_WAIT:
  case FR_MEDIA_ERROR_EOF:
    return FR_JOB_STATE_PAUSE;
    break;
  default:
    return FR_JOB_STATE_PAUSE;
  }
}

static FR_JOB_STATE_ENUM decoder_process(FrDecoder* self) {
  SysInt err = 0;
  FR_JOB_STATE_ENUM state;

  err = decoder_process_packet(self);

  state = error_to_state(err);

  if (state < 0) {
    sys_warning_N("process packet failed: %s,%s", 
      self->name,
      fr_media_error_string(err));
  }

  return state;
}

static SysPointer decoder_thread(SysPointer user_data) {
  FrDecoder *self = user_data;

  decoder_process(self);

  return NULL;
}

void fr_decoder_wakeup_unlock(FrDecoder* self) {
  sys_return_if_fail(self != NULL);

  self->state = FR_JOB_STATE_RUNNING;
  sys_cond_signal(&self->ctrl.cond);
}

static SysPointer init_it(FrJobTask* task, SysPointer user_data) {
  FrDecoder *self = user_data;

  sys_debug_N("init it: %s", self->name);
  return NULL;
}

void decoder_wait_init(FrDecoder* self) {
  FrJobTask task = { 0 };
  task.handler = init_it;
  task.user_data = self;

  fr_job_run_task_wait(&self->job, &task);
}

SysInt fr_decoder_start(FrDecoder* self) {
  const SysChar *name = self->name;

  fr_job_start(&self->job, name, decoder_thread);
  decoder_wait_init(self);

  return 0;
}

void decoder_done(FrJobTask task) {
}

void fr_decoder_stop(FrDecoder* self) {
  sys_return_if_fail(self != NULL);
  if (fr_decoder_get_state(self) == FR_JOB_STATE_STOP) { return; }

  fr_job_stop(&self->job);
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

  if(!sys_queue_get_length(&self->queue)) {
    return false;
  }

  npkt = sys_queue_pop_tail(&self->queue);
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

  sys_queue_push_head (&self->queue, pkt);
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

  return FR_MEDIA_ERROR_SUCCESS;
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

  fr_job_stop(&self->job);
  sys_queue_clear(&self->queue);
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
  self->min_pkt = 24;
  self->max_pkt = 96;

  sys_queue_init(&self->queue);
  fr_job_create(&self->job);
}
