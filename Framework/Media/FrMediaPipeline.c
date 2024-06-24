#include <Framework/Media/FrMediaPipeline.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrMediaPlayer.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Event/FrEvents.h>
#include <Framework/Event/Base/FrEventRefresh.h>

SYS_DEFINE_TYPE(FrMediaPipeline, fr_media_pipeline, SYS_TYPE_OBJECT);


static FrDecoder *pipeline_get_decoder(FrMediaPipeline *self,
    FR_MEDIA_ENUM type) {
  sys_return_val_if_fail(self != NULL, NULL);

  switch (type) {
  case FR_MEDIA_VIDEO:
    return self->video_decoder;
  case FR_MEDIA_AUDIO:
    return self->audio_decoder;
  default:
    return NULL;
  }
}

static void pipeline_push_sample_frame(FrMediaPipeline* self,
    FrMediaFrame* frame) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(frame != NULL);

  /**
   * NOTE: ignore leak, vld cannot detect malloc this thread,
   * and free on other thread.
   */
  sys_async_queue_push(&self->sample_queue, frame);
}

static void pipeline_push_image_frame(FrMediaPipeline* self,
    FrMediaFrame* frame) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(frame != NULL);

  /**
   * NOTE: ignore leak, vld cannot detect malloc this thread,
   * and free on other thread.
   */
  sys_async_queue_push(&self->image_queue, frame);
}

static SysPointer decode_frame(
    FrTask* o,
    SysPointer user_data) {

  SysPointer *pass = user_data;

  SysInt err;
  FrMediaPipeline *self = pass[0];
  FrMediaDecoder *mdec = pass[1];
  FrMediaPacket *mpkt = pass[2];
  FrMediaFrame *mframe;

  err = fr_media_decoder_send_packet(mdec, mpkt);
  if(err < 0) { return NULL; }

  err = fr_media_decoder_try_decode_frame(mdec, &mframe);
  if(err < 0) { return NULL; }

  fr_media_decoder_push_frame(mdec, mframe);

  sys_free_N(pass);

  return NULL;
}

static SysInt pipeline_decode_frame(
    FrMediaPipeline *self,
    FrDecoder *dec,
    FrPacket *pkt) {

  SysPointer *pass = sgc_type_new(SYS_TYPE_POINTER, 2);

  pass[0] = self;
  pass[1] = dec;
  pass[1] = pkt;

  FrTask *task = fr_task_new_handler(decode_frame, pass);
  fr_job_run_task_async(&dec->job, task);

  return 0;
}

static SysPointer process_packet(
    FrTask* o,
    SysPointer user_data) {

  SysInt err;
  FrPacket *npkt = NULL;
  FrMediaPipeline *self = user_data;
  FrDecoder *dec = self->packet_decoder;
  FrDecoder *todec;

  FrMediaPacket *mpkt;
  SysInt sindex;

  err = fr_packet_decoder_decode(dec, &npkt);
  if(err < 0) { return NULL; }

  mpkt = FR_MEDIA_PACKET(npkt);
  sindex = fr_media_packet_get_stream_index(mpkt);

  todec = pipeline_get_decoder(self, sindex);
  if(todec == NULL) {
    sys_warning_N("Not found media packet type: %s", sindex);
    return NULL;
  }

  err = pipeline_decode_frame(self, todec, npkt);
  if(err < 0) { return NULL; }

  return NULL;
}

static FrDecoder* create_media_decoder(FrMediaFile* file,
    FR_MEDIA_ENUM mediaType,
    FrMediaPipeline *self) {

  sys_return_val_if_fail(file != NULL, false);

  FrDecoder* decoder;

  decoder = fr_media_decoder_create_by_media_type(file, mediaType);
  if (decoder == NULL) { return false; }
  fr_decoder_set_user_data(decoder, self);

  if (fr_decoder_open(decoder) > 0) {
    sys_clear_pointer(&decoder, _sys_object_unref);
    return NULL;
  }

  return decoder;
}

static void packet_decoder_run(FrMediaPipeline *self) {
  FrDecoder *dec = self->packet_decoder;

  FrTask *task = fr_task_new_handler(process_packet, self);
  fr_job_run_task_async(&dec->job, task);
}

void fr_media_pipeline_run(FrMediaPipeline *self, FrMediaFile *file) {
  FrDecoder *vdec;
  FrDecoder *adec;
  FrDecoder *pdec;
  FrVideoDecoder *video_dec;

  pdec = fr_packet_decoder_new_I(file);
  fr_decoder_set_user_data(pdec, self);
  self->packet_decoder = pdec;

  vdec = create_media_decoder(file,
      FR_MEDIA_VIDEO,
      self);
  video_dec = FR_VIDEO_DECODER(vdec);
  fr_video_decoder_resize(video_dec, 800, 600);
  self->video_decoder = vdec;

  adec = create_media_decoder(file,
      FR_MEDIA_AUDIO,
      self);
  self->audio_decoder = adec;

  fr_decoder_start(self->video_decoder);
  fr_decoder_start(self->audio_decoder);
  fr_decoder_start(self->packet_decoder);

  packet_decoder_run(self);
}

FrMediaFrame* fr_media_pipeline_get_image_frame (FrMediaPipeline* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return sys_async_queue_try_pop(&self->image_queue);
}

void fr_media_pipeline_get_video_size(FrMediaPipeline *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  FrVideoDecoder *video_decoder = FR_VIDEO_DECODER(self->video_decoder);
  if(video_decoder == NULL) { return; }

  fr_video_decoder_get_size(video_decoder, width, height);
}

SysBool fr_media_pipeline_destroy_i(SysObject *o) {
  FrMediaPipeline *self = FR_MEDIA_PIPELINE(o);

  fr_decoder_stop(self->packet_decoder);
  sys_clear_pointer(&self->packet_decoder, _sys_object_unref);

  fr_decoder_stop(self->video_decoder);
  sys_clear_pointer(&self->video_decoder, _sys_object_unref);

  fr_decoder_stop(self->audio_decoder);
  sys_clear_pointer(&self->audio_decoder, _sys_object_unref);

  sys_async_queue_clear_full(&self->image_queue);
  sys_async_queue_clear_full(&self->sample_queue);

  return true;
}

void fr_media_pipeline_stop_player(FrMediaPipeline *self) {

  fr_media_player_set_state(self->player, FR_JOB_STATE_STOP);
}

void fr_media_pipeline_wakeup_source(FrMediaPipeline *self,
    FrDecoder *dec) {

  if(!fr_decoder_enough(dec)) {
  }
}

/* object api */
static void fr_media_pipeline_construct_i(FrMediaPipeline *self) {

}

FrMediaPipeline* fr_media_pipeline_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_PIPELINE, NULL);
}

FrMediaPipeline *fr_media_pipeline_new_I(void) {
  FrMediaPipeline *o = fr_media_pipeline_new();

  fr_media_pipeline_construct_i(o);

  return o;
}

static void fr_media_pipeline_dispose(SysObject* o) {
  fr_media_pipeline_destroy_i(o);

  SYS_OBJECT_CLASS(fr_media_pipeline_parent_class)->dispose(o);
}

static void fr_media_pipeline_class_init(FrMediaPipelineClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_media_pipeline_dispose;
  ocls->destroy = fr_media_pipeline_destroy_i;
}

void fr_media_pipeline_init(FrMediaPipeline* self) {
  sys_async_queue_init_full(&self->image_queue, (SysDestroyFunc)_sys_object_unref);
  sys_async_queue_init_full(&self->sample_queue, (SysDestroyFunc)_sys_object_unref);
}
