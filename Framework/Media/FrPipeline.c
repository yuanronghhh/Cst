#include <Framework/Media/FrPipeline.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrVideoDecoder.h>

SYS_DEFINE_TYPE(FrPipeline, fr_pipeline, SYS_TYPE_OBJECT);

FrDecoder *fr_pipeline_get_decoder(FrPipeline *self, FR_MEDIA_ENUM type) {
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
    FrPipeline *self) {

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

void fr_pipeline_run(FrPipeline *self, FrMediaFile *file) {
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

SysBool fr_pipeline_destroy_i(SysObject *o) {
  FrPipeline *self = FR_PIPELINE(o);

  sys_clear_pointer(&self->packet_decoder, _sys_object_unref);
  sys_clear_pointer(&self->video_decoder, _sys_object_unref);
  sys_clear_pointer(&self->audio_decoder, _sys_object_unref);

  sys_async_queue_clear(&self->image_queue);
  sys_async_queue_clear(&self->sample_queue);

  return true;
}

/* object api */
static void fr_pipeline_construct(FrPipeline *self) {
}

FrPipeline* fr_pipeline_new(void) {
  return sys_object_new(FR_TYPE_PIPELINE, NULL);
}

FrPipeline *fr_pipeline_new_I(void) {
  FrPipeline *o = fr_pipeline_new();

  fr_pipeline_construct(o);

  return o;
}

static void fr_pipeline_dispose(SysObject* o) {
  FrPipeline *self = FR_PIPELINE(o);

  sys_clear_pointer(&self->image_queue, sys_async_queue_clear);
  sys_clear_pointer(&self->sample_queue, sys_async_queue_clear);

  SYS_OBJECT_CLASS(fr_pipeline_parent_class)->dispose(o);
}

static void fr_pipeline_class_init(FrPipelineClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_pipeline_dispose;
  ocls->destroy = fr_pipeline_destroy_i;
}

void fr_pipeline_init(FrPipeline* self) {
  sys_async_queue_init(&self->image_queue);
  sys_async_queue_init(&self->sample_queue);
}
