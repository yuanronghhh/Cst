#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrIDecoder.h>
#include <Framework/DataType/FrMain.h>

SYS_DEFINE_TYPE(FrDecoder, fr_decoder, FR_TYPE_JOB);

#define DECODER_LOCK fr_job_lock(&self->job)
#define DECODER_UNLOCK fr_job_unlock(&self->job)

const SysChar* fr_decoder_get_name(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

SysBool fr_decoder_enough(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, false);
  // SysInt r;
  // SysUInt len;
  // FrDecoder *o = FR_DECODER(self);

  // len = fr_decoder_get_length(o);
  // r = len >= self->max_pkt;

  // return r;
  
  return true;
}

SysBool fr_decoder_not_enough(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, false);
  // FrDecoder *o = FR_DECODER(self);
  // SysUInt len = fr_decoder_get_length(o);

  // return len <= self->min_pkt;

  return true;
}

static SysInt fr_decoder_decode_it_i(FrDecoder *self, FrPacket *pkt) {
  sys_return_val_if_fail(self != NULL, -1);

  return FR_MEDIA_ERROR_WAIT;
}

void fr_decoder_wakeup(FrDecoder *self) {
  sys_return_if_fail(self != NULL);

  fr_job_wakeup(&self->job);
}

SysBool fr_decoder_pop_packet_unlock(FrDecoder* self,
    FrPacket **pkt) {
  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(*pkt == NULL, -1);
  FrPacket *npkt;

#if 0
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
#endif

  return true;
}

SysBool fr_decoder_pop_packet(FrDecoder* self,
    FrPacket** pkt) {
  SysBool r;

  r = fr_decoder_pop_packet_unlock(self, pkt);

  return r;
}

void fr_decoder_push_packet(FrDecoder* self, FrPacket* pkt) {

  sys_async_queue_push(&self->queue, pkt);
}

static void fr_decoder_process_packet(FrJob* o, SysPointer user_data) {
  SysInt err;
  FrPacket *npkt = NULL;
  FrDecoder *self = FR_DECODER(o);

  err = fr_decoder_decode_check(self);
  if(err < 0) { return; }

  if(!fr_decoder_pop_packet_unlock(self, &npkt)) {

    err = FR_MEDIA_ERROR_WAIT;
  }
  err = fr_decoder_decode_it(self, npkt);

  // if(fr_decoder_get_length(self) > 0) {

  //   fr_decoder_wakeup(self);
  // }
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

static SysPointer decoder_init(FrJobTask* o, SysPointer user_data) {
  FrDecoder *self = FR_DECODER(o);

  FrMain *loop = fr_main_new_I();
  fr_main_run(loop);

  return NULL;
}

SysInt fr_decoder_start(FrDecoder* self) {
  FrJobTask *task;

  fr_job_start(&self->job);

  task = fr_job_task_new_handler(decoder_init, self);
  fr_job_run_task_async(&self->job, task);

  return 0;
}

void fr_decoder_stop(FrDecoder* self) {
  sys_return_if_fail(self != NULL);

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

SysInt fr_decoder_open_i(FrDecoder* o) {
  return 0;
}

SysInt fr_decoder_close_i(FrDecoder* o) {
  return 0;
}

/* object api */
static void fr_decoder_construct_i(FrDecoder *self,
    FrDecoderContext *info) {

  self->name = sys_strdup(info->name);
  fr_job_create(&self->job);

  FrJobContext jinfo = {0};
  jinfo.name = self->name;
  jinfo.user_data = self;

  fr_job_construct(&self->job, &jinfo);
}

FrDecoder* fr_decoder_new(void) {
  return sys_object_new(FR_TYPE_DECODER, NULL);
}

static void fr_decoder_dispose(SysObject* o) {
  FrDecoder *self = FR_DECODER(o);

  fr_job_stop(&self->job);
  sys_free_N(self->name);

  SYS_OBJECT_CLASS(fr_decoder_parent_class)->dispose(o);
}

static void fr_decoder_class_init(FrDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->open = fr_decoder_open_i;
  cls->close = fr_decoder_close_i;
  cls->decode_it = fr_decoder_decode_it_i;
  cls->decode_check = fr_decoder_decode_check_i;
  cls->construct = fr_decoder_construct_i;

  ocls->dispose = fr_decoder_dispose;
}

void fr_decoder_init(FrDecoder* self) {
  self->start_pts = -1;
  self->serial = -1;
  self->min_pkt = 1;
  self->max_pkt = 10;
}
