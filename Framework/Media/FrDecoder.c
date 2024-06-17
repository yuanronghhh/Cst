#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrIDecoder.h>

SYS_DEFINE_TYPE(FrDecoder, fr_decoder, FR_TYPE_JOB);

#define DECODER_LOCK fr_job_lock(FR_JOB(self))
#define DECODER_UNLOCK fr_job_unlock(FR_JOB(self))

const SysChar* fr_decoder_get_name(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

SysBool fr_decoder_enough(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, false);
  SysInt r;
  SysUInt len;
  FrDecoder *o = FR_DECODER(self);

  len = fr_decoder_get_length(o);
  r = len >= self->max_pkt;

  return r;
}

SysBool fr_decoder_not_enough(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, false);
  FrDecoder *o = FR_DECODER(self);
  SysUInt len = fr_decoder_get_length(o);

  return len <= self->min_pkt;
}

static SysInt fr_decoder_decode_it_i(FrDecoder *self, FrPacket *pkt) {
  sys_return_val_if_fail(self != NULL, -1);

  return FR_MEDIA_ERROR_WAIT;
}

void fr_decoder_set_task(FrDecoder* self, FrJobTask* task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  fr_job_send_task(FR_JOB(self), task);
}

void fr_decoder_wakeup(FrDecoder *self) {
  sys_return_if_fail(self != NULL);

  fr_job_wakeup(FR_JOB(self));
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

  r = fr_decoder_pop_packet_unlock(self, pkt);

  return r;
}

void fr_decoder_push_packet_unlock(FrDecoder* self, FrPacket* pkt) {
  FrJobTask *task = fr_job_task_new_handler(NULL, pkt);

  fr_job_send_task(FR_JOB(self), task);
}

void fr_decoder_push_packet(FrDecoder* self, FrPacket* pkt) {
  DECODER_LOCK;
  fr_decoder_push_packet_unlock(self, pkt);
  DECODER_UNLOCK;
}

static SysInt decoder_process_packet(FrDecoder* self) {
  SysInt err;
  FrPacket *npkt = NULL;

  err = fr_decoder_decode_check(self);
  if(err < 0) { return err; }

  if(!fr_decoder_pop_packet_unlock(self, &npkt)) {

    err = FR_MEDIA_ERROR_WAIT;
    goto done;
  }
  err = fr_decoder_decode_it(self, npkt);

  if(fr_decoder_get_length(self) > 0) {

    fr_decoder_wakeup(self);
  }

done:
  return err;
}

#if 0
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
#endif

static SysPointer job_callback(FrJob* job, SysPointer user_data) {
  FrDecoder *self = user_data;

  decoder_process_packet(self);

  return NULL;
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

  fr_job_send_task_wait(FR_JOB(self), &task);
}

SysInt fr_decoder_start(FrDecoder* self) {

  fr_job_start(FR_JOB(self));
  decoder_wait_init(self);

  return 0;
}

void decoder_done(FrJobTask task) {
}

void fr_decoder_stop(FrDecoder* self) {
  sys_return_if_fail(self != NULL);

  fr_job_stop(FR_JOB(self));
}

void fr_decoder_set_user_data(FrDecoder *self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  self->user_data = user_data;
}

SysPointer fr_decoder_get_user_data(FrDecoder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->user_data;
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
static void fr_job_construct_i(FrJob *o,
    FrJobContext *info) {

  FR_JOB_CLASS(fr_decoder_parent_class)->construct(o, info);
}

FrDecoder* fr_decoder_new(void) {
  return sys_object_new(FR_TYPE_DECODER, NULL);
}

static void fr_decoder_dispose(SysObject* o) {
  FrDecoder *self = FR_DECODER(o);

  fr_job_stop(FR_JOB(self));
  sys_free_N(self->name);

  SYS_OBJECT_CLASS(fr_decoder_parent_class)->dispose(o);
}

static void fr_decoder_class_init(FrDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrJobClass *jcls = FR_JOB_CLASS(cls);

  cls->open = fr_decoder_open_i;
  cls->close = fr_decoder_close_i;
  cls->decode_it = fr_decoder_decode_it_i;
  cls->decode_check = fr_decoder_decode_check_i;

  jcls->construct = fr_job_construct_i;

  ocls->dispose = fr_decoder_dispose;
}

void fr_decoder_init(FrDecoder* self) {
  self->start_pts = -1;
  self->serial = -1;
  self->min_pkt = 1;
  self->max_pkt = 10;
}
