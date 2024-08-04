#include <Framework/Media/FrMediaPipeline.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrMediaPlayer.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Event/FrEvents.h>
#include <Framework/Event/Base/FrEventRefresh.h>
#include <Framework/Media/FrImageSaver.h>

typedef struct _PipePass PipePass;

struct _PipePass {
  FrMediaPipeline *pipe;
  FrDecoder* pdec;
  FrDecoder* todec;
  FrPacket* pkt;
  SysAsyncQueue *queue;
};

SYS_DEFINE_TYPE(FrMediaPipeline, fr_media_pipeline, SYS_TYPE_OBJECT);

static PipePass* pipe_pass_new_by_type(
    FrMediaPipeline *pipe,
    FR_MEDIA_ENUM type,
    FrPacket *pkt) {
  sys_return_val_if_fail(pipe != NULL, NULL);
  PipePass *pass = sgc_malloc0(sizeof(PipePass));

  pass->pkt = pkt;
  pass->pipe = pipe;
  switch (type) {
    case FR_MEDIA_VIDEO:
      pass->todec = pipe->video_decoder;
      pass->queue = &pipe->image_queue;
      break;
    case FR_MEDIA_AUDIO:
      pass->todec = pipe->audio_decoder;
      pass->queue = &pipe->sample_queue;
      break;
    case FR_MEDIA_SUBTITLE:
      pass->todec = pipe->subtitle_decoder;
      pass->queue = &pipe->subtitle_queue;
      break;
    default:
      return NULL;
  }

  return pass;
}

static void pipe_pass_free(PipePass *self) {

  sys_object_unref(self->pkt);
  sys_free_N(self);
}

static SysPointer decode_frame(
    FrTask* o,
    SysPointer user_data) {
  PipePass *pass = user_data;

  SysInt err;
  FrMediaDecoder *mdec = FR_MEDIA_DECODER(pass->todec);
  FrMediaPacket *mpkt = FR_MEDIA_PACKET(pass->pkt);
  SysAsyncQueue *queue = pass->queue;
  FrMediaFrame *mframe = NULL;
  FrMediaFrame *nframe = NULL;

  err = fr_media_decoder_send_packet(mdec, mpkt);
  if(err < 0) { goto fail; }

  err = fr_media_decoder_try_decode_frame(mdec, &mframe);
  if(err < 0) { goto fail; }

  nframe = (FrMediaFrame *)sys_object_dclone(mframe);
  sys_async_queue_push(queue, nframe);

  pipe_pass_free(pass);
  return NULL;

fail:
  sys_atomic_int_dec(&pass->pipe->pkt_count);
  pipe_pass_free(pass);

  return NULL;
}

static SysPointer process_packet(
    FrTask* o,
    SysPointer user_data) {

  SysInt err;
  FrMediaPacket *mpkt;
  SysInt sindex;
  PipePass *pass = NULL;

  FrPacket *npkt = NULL;
  FrMediaPipeline *pipe = user_data;

  err = fr_packet_decoder_decode(pipe->packet_decoder, &npkt);
  if(err < 0) { return NULL; }

  mpkt = FR_MEDIA_PACKET(npkt);
  sindex = fr_media_packet_get_stream_index(mpkt);
  if(sindex < 0) { goto fail; }

  pass = pipe_pass_new_by_type(pipe, sindex, npkt);
  if(pass == NULL) {

    sys_warning_N("Not found media packet type: %s", sindex);
    return NULL;
  }
  fr_decoder_run_async(pass->todec, decode_frame, pass);

  return NULL;
fail:
  if(npkt != NULL) {

    sys_clear_pointer(&npkt, _sys_object_unref);
  }

  if(pass != NULL) {

    sys_clear_pointer(&pass, pipe_pass_free);
  }

  return NULL;
}

static FrDecoder* create_media_decoder(FrMediaFile* file,
    FR_MEDIA_ENUM mediaType,
    FrMediaPipeline *self) {

  sys_return_val_if_fail(file != NULL, NULL);
  FrDecoder* decoder;

  decoder = fr_media_decoder_create_by_media_type(file, mediaType);
  if (decoder == NULL) { return NULL; }
  fr_decoder_set_user_data(decoder, self);

  if (fr_decoder_open(decoder) > 0) {

    sys_clear_pointer(&decoder, _sys_object_unref);
    return NULL;
  }

  return decoder;
}

void fr_media_pipeline_run(FrMediaPipeline *self, FrMediaFile *file) {
  FrDecoder *vdec;
  FrDecoder *adec;
  FrDecoder *pdec;
  FrVideoDecoder *video_dec;

  pdec = fr_packet_decoder_new_I(file);
  fr_decoder_set_user_data(pdec, self);

  self->packet_decoder = pdec;
  fr_decoder_start(self->packet_decoder);

  vdec = create_media_decoder(file,
      FR_MEDIA_VIDEO,
      self);
  if(vdec != NULL) {

    video_dec = FR_VIDEO_DECODER(vdec);
    fr_video_decoder_resize(video_dec, 800, 600);

    self->video_decoder = vdec;
    fr_decoder_start(self->video_decoder);
  }

  adec = create_media_decoder(file,
      FR_MEDIA_AUDIO,
      self);
  if(adec != NULL) {

    self->audio_decoder = adec;
    fr_decoder_start(self->audio_decoder);
  }
}

FrMediaFrame* fr_media_pipeline_get_sample_frame (FrMediaPipeline* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FrMediaFrame *sample = sys_async_queue_try_pop(&self->sample_queue);
  if (sample == NULL) { return NULL; }
  sys_atomic_int_dec(&self->pkt_count);

  return sample;
}

FrMediaFrame* fr_media_pipeline_get_image_frame (FrMediaPipeline* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FrMediaFrame *image = sys_async_queue_try_pop(&self->image_queue);
  if (image == NULL) { return NULL; }
  sys_atomic_int_dec(&self->pkt_count);

  return image;
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
  self->pkt_count = 0;

  return true;
}

static void stop_player(FrMediaPipeline *self) {
  sys_debug_N("%s", "stop player");

  fr_media_player_set_state(self->player, FR_JOB_STATE_STOP);
}

void fr_media_pipeline_wakeup_source(FrMediaPipeline *self) {
  sys_return_if_fail(self != NULL);

  if(fr_decoder_get_eof(self->packet_decoder)) {
    stop_player(self);
    return;
  }

  if (self->pkt_count > self->min_packet) {
    return;
  }
  sys_debug_N("wakeup %ld", self->pkt_count);

  for(int i = 0; i < self->max_packet; i++) {

    fr_decoder_run_async(self->packet_decoder, process_packet, self);
    sys_atomic_int_inc(&self->pkt_count);
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
  self->min_packet = 2;
  self->max_packet = 30 * self->min_packet;
  self->pkt_count = 0;

  sys_async_queue_init_full(&self->image_queue, (SysDestroyFunc)_sys_object_unref);
  sys_async_queue_init_full(&self->sample_queue, (SysDestroyFunc)_sys_object_unref);
}
