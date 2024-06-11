#include <Framework/Media/FrMediaPipeline.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Event/FrEvents.h>
#include <Framework/Event/Base/FrEventRefresh.h>

SYS_DEFINE_TYPE(FrMediaPipeline, fr_media_pipeline, FR_TYPE_PIPELINE);

FrDecoder *fr_media_pipeline_get_decoder(FrMediaPipeline *self, FR_MEDIA_ENUM type) {
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

static FrDecoder* run_media_decoder(FrMediaFile* file,
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
  fr_decoder_start(decoder);

  return decoder;
}

void fr_media_pipeline_run(FrMediaPipeline *self, FrMediaFile *file) {
  FrDecoder *dec;
  FrVideoDecoder *vdec;

  dec = run_media_decoder(file,
      FR_MEDIA_VIDEO,
      self);
  vdec = FR_VIDEO_DECODER(dec);
  fr_video_decoder_resize(vdec, 800, 600);

  self->video_decoder = dec;

  dec = run_media_decoder(file,
      FR_MEDIA_AUDIO,
      self);
  self->audio_decoder = dec;

  dec = fr_packet_decoder_new_I(file);
  fr_decoder_set_user_data(dec, self);
  self->packet_decoder = dec;

  fr_decoder_start(self->packet_decoder);
}

void fr_media_pipeline_push_sample_frame(FrMediaPipeline* self,
    FrMediaFrame* frame) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(frame != NULL);

  /**
   * NOTE: ignore leak, vld cannot detect malloc this thread,
   * and free on other thread.
   */
  sys_async_queue_push(&self->sample_queue, frame);
}

void fr_media_pipeline_push_image_frame(FrMediaPipeline* self,
    FrMediaFrame* frame) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(frame != NULL);

  /**
   * NOTE: ignore leak, vld cannot detect malloc this thread,
   * and free on other thread.
   */
  sys_async_queue_push(&self->image_queue, frame);
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
