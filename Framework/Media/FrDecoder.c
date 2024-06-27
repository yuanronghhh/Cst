#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrIDecoder.h>
#include <Framework/DataType/FrMain.h>

SYS_DEFINE_TYPE(FrDecoder, fr_decoder, SYS_TYPE_OBJECT);

#define DECODER_LOCK sys_async_queue_lock(&self->queue)
#define DECODER_UNLOCK sys_async_queue_unlock(&self->queue)

const SysChar* fr_decoder_get_name(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

static SysPointer decoder_init(FrTask* o, SysPointer user_data) {
  return NULL;
}

SysInt fr_decoder_start(FrDecoder* self) {
  FrTask *task;

  fr_job_start(&self->job);

  task = fr_task_new_handler(decoder_init, self);
  fr_job_run_task_sync(&self->job, task);

  return 0;
}

void fr_decoder_stop(FrDecoder* self) {
  sys_return_if_fail(self != NULL);

  fr_job_stop(&self->job);
}

void fr_decoder_run_async(FrDecoder *self,
    FrTaskFunc func,
    SysPointer user_data) {
  FrTask *task = fr_task_new_handler(func, user_data);
  fr_job_run_task_async(&self->job, task);
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
  fr_job_join(&self->job);
  sys_object_destroy(&self->job);
  sys_free_N(self->name);

  SYS_OBJECT_CLASS(fr_decoder_parent_class)->dispose(o);
}

static void fr_decoder_class_init(FrDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->open = fr_decoder_open_i;
  cls->close = fr_decoder_close_i;
  cls->construct = fr_decoder_construct_i;

  ocls->dispose = fr_decoder_dispose;
}

void fr_decoder_init(FrDecoder* self) {
  self->start_pts = -1;
  self->serial = -1;
  self->min_pkt = 1;
  self->max_pkt = 10;
}
