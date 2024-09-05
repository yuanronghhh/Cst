#include <Framework/Media/FrAvPlayer.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrFrameDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrIMediaRender.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrAudioFrame.h>
#include <Framework/Media/FrAvRender.h>
#include <Framework/Device/FrWindow.h>

typedef struct _PipePass PipePass;

struct _PipePass {
  FrDecoder* todec;
  FrPacket* pkt;
  FrAvPlayer *pipe;
};

SYS_DEFINE_TYPE(FrAvPlayer, fr_av_player, FR_TYPE_PLAYER);

static void pipe_pass_free(PipePass *self) {

  sys_clear_pointer(&self->pkt, _sys_object_unref);
  sys_free(self);
}

static PipePass* pipe_pass_new_by_type(
    FrAvPlayer *pipe,
    FR_MEDIA_ENUM type,
    FrPacket *pkt) {
  sys_return_val_if_fail(pipe != NULL, NULL);

  PipePass *pass = sys_new0(PipePass, 1);

  pass->pkt = pkt;
  pass->pipe = pipe;
  switch (type) {
    case FR_MEDIA_VIDEO:
      pass->todec = pipe->video_decoder;
      break;
    case FR_MEDIA_AUDIO:
      pass->todec = pipe->audio_decoder;
      break;
    case FR_MEDIA_SUBTITLE:
      pass->todec = pipe->subtitle_decoder;
      break;
    default:
      return NULL;
  }

  if(pass->todec == NULL) {
    goto fail;
  }

  return pass;

fail:
  sys_free(pass);
  return NULL;
}

static SysPointer decode_frame(
    FrTask* o,
    SysPointer user_data) {
  PipePass *pass = user_data;

  SysInt err;
  FrMediaDecoder *mdec = FR_MEDIA_DECODER(pass->todec);
  FrMediaPacket *mpkt = FR_MEDIA_PACKET(pass->pkt);
  FrMediaFrame *mframe = NULL;
  FrMediaPacket *nframe = NULL;

  err = fr_media_decoder_send_packet(mdec, mpkt);
  if(err < 0) { goto done; }

  err = fr_media_decoder_try_decode_frame(mdec, &mframe);
  if(err < 0) { goto done; }

  nframe = (FrMediaPacket *)sys_object_dclone(mframe);
  fr_media_decoder_write(mdec, nframe);

done:
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
  FrAvPlayer *pipe = user_data;

  err = fr_decoder_decode(pipe->packet_decoder, &npkt);
  if(err < 0) { return NULL; }

  mpkt = FR_MEDIA_PACKET(npkt);
  sindex = fr_media_packet_get_stream_index(mpkt);
  if(sindex < 0) {

    sys_clear_pointer(&npkt, _sys_object_unref);
    return NULL;
  }

  pass = pipe_pass_new_by_type(pipe, sindex, npkt);
  if(pass == NULL) { return NULL; }

  fr_decoder_run_async(pass->todec, decode_frame, pass);

  return NULL;
}

static FrDecoder* create_media_decoder(FrMediaFile* file,
    FR_MEDIA_ENUM mediaType,
    FrAvPlayer *self) {

  sys_return_val_if_fail(file != NULL, NULL);
  FrDecoder* decoder;

  decoder = fr_media_decoder_create_by_media_type(file, mediaType);
  if (decoder == NULL) { return NULL; }
  fr_decoder_set_user_data(decoder, self);

  if (fr_decoder_open(decoder) > 0) {

    sys_clear_pointer(&decoder, _sys_object_unref);
    return NULL;
  }
  fr_decoder_start(decoder);

  return decoder;
}

void av_player_get_video_size(FrAvPlayer *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  FrVideoDecoder *video_decoder = FR_VIDEO_DECODER(self->video_decoder);
  if(video_decoder == NULL) { return; }

  fr_video_decoder_get_size(video_decoder, width, height);
}

static void av_player_create_decoder(FrAvPlayer *self, FrMediaFile *file) {
  FrVideoDecoder *video_dec;

  self->packet_decoder = fr_packet_decoder_new_I(file);
  fr_decoder_set_user_data(self->packet_decoder, self);

  self->video_decoder = create_media_decoder(file,
      FR_MEDIA_VIDEO,
      self);

  video_dec = FR_VIDEO_DECODER(self->video_decoder);
  if(video_dec) {

    fr_video_decoder_resize(video_dec, 800, 600);
  }

  self->audio_decoder = create_media_decoder(file,
      FR_MEDIA_AUDIO,
      self);
}

static void calc_video_delay (FrAvPlayer *self) {
  FrMediaStream* vs;
  FrRational rt = { .num = 2997, .den = 100 };

  vs = fr_media_file_stream_by_type(self->file, FR_MEDIA_VIDEO);
  fr_media_stream_get_rational(vs, &rt);
  self->delay = self->default_delay = (1 / (rt.num / (double) rt.den)) * 1.0e6;
}

static SysInt fr_av_player_init_i(FrPlayer *o) {
  FrAvPlayer *self = FR_AV_PLAYER(o);
  FrBound bound = { .width = 800, .height = 600 };

  fr_player_set_state(o, FR_JOB_STATE_RUNNING);

  if(fr_media_file_has_video(self->file)) {
    calc_video_delay(self);
  }

  av_player_get_video_size(self, &bound.width, &bound.height);
  fr_window_set_size(self->window, bound.width, bound.height);

  av_player_create_decoder(self, self->file);

  self->region = fr_region_create_rectangle(&bound);

  return 0;
}

static SysInt fr_av_player_process_i (FrPlayer *o) {
  FrAvPlayer *self = FR_AV_PLAYER(o);
  FrDecoder *dec = FR_DECODER(self->packet_decoder);

  if(fr_decoder_get_eof(dec)) {

    fr_player_set_state(o, FR_JOB_STATE_STOP);
    return 0;
  }

  if (self->pkt_count <= self->min_packet) {
    for(int i = 0; i < self->max_packet; i++) {

      fr_decoder_run_async(dec, process_packet, self);
      sys_atomic_int_inc(&self->pkt_count);
    }
  }

  fr_av_player_render(self, self->render, self->region);
  fr_poll_events();
  fr_delay(self->delay);

  return 0;
}

static SysInt fr_av_player_stop_i (FrPlayer *o) {
  FrAvPlayer *self = FR_AV_PLAYER(o);

  fr_region_destroy(self->region);

  return 0;
}

void fr_av_player_set_render(FrAvPlayer* self, FrAvRender *render) {
  sys_return_if_fail(self != NULL);

  self->render = render;
}

void fr_av_player_set_window(FrAvPlayer *self, FrWindow * window) {
  sys_return_if_fail(self != NULL);

  self->window = window;
}

FrWindow * fr_av_player_get_window(FrAvPlayer *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->window;
}

static void media_player_calc_diff(FrAvPlayer *self) {
#if 0
  SysInt64 diff = 0;

  if(fr_media_file_has_audio(self->file)) {

    if(fr_media_file_has_video(self->file)) {

       diff = self->vtsp - self->base_tsp;
    }
  } else if(fr_media_file_has_audio(self->file)) {

  } else {
  }

  self->delay = diff <= 0 ? self->default_delay : diff / 1.0e3;
  sys_debug_N("%ld", self->delay);
#endif
}

SysInt fr_av_player_sync(FrAvPlayer *self) {

  return -1;
}

SysInt fr_av_player_render(FrAvPlayer *self,
    FrAvRender *render,
    FrRegion *region) {
  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(render != NULL, -1);
  sys_return_val_if_fail(region != NULL, -1);

  FrMediaFrame *frame = NULL;
  FrVideoFrame *vframe = NULL;
  FrMediaDecoder *mdec;

#if 0
  FrAudioFrame *aframe = NULL;
  frame = fr_media_pipeline_get_sample_frame(&self->pipeline);
  if (frame != NULL) {
    aframe = FR_AUDIO_FRAME(frame);
    self->base_tsp = fr_media_frame_get_timestamp(frame);

    fr_av_render_render_audio(render, aframe);
    sys_object_unref(aframe);
  }
#endif

  mdec = FR_MEDIA_DECODER(self->video_decoder);
  fr_media_decoder_read(mdec, (FrMediaPacket **)&frame);
  if (frame != NULL) {
    self->vtsp = fr_media_frame_get_timestamp(frame);

    if(!fr_media_file_has_audio(self->file)) {

      self->base_tsp = self->vtsp;
    }

    vframe = FR_VIDEO_FRAME(frame);

    fr_av_render_render_video(render, vframe, region);
    sys_object_unref(vframe);
  }

  // media_player_calc_diff(self);

  return FR_MEDIA_ERROR_EOF;
}

void fr_av_player_play(FrAvPlayer* self) {
  sys_return_if_fail(self != NULL);

  fr_media_file_play(self->file);
}

/* object api */
static void fr_av_player_construct(FrPlayer *o, FrAvPlayerContext *info) {
  FrAvPlayer *self = FR_AV_PLAYER(o);

  self->file = sys_object_ref(info->file);
  self->window = sys_object_ref(info->window);
  self->render = sys_object_ref(info->render);
}

FrPlayer* fr_av_player_new(void) {
  return sys_object_new(FR_TYPE_AV_PLAYER, NULL);
}

FrPlayer *fr_av_player_new_I(FrAvPlayerContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->file != NULL, NULL);
  sys_return_val_if_fail(info->window != NULL, NULL);
  sys_return_val_if_fail(info->render != NULL, NULL);

  FrPlayer *o = fr_av_player_new();
  fr_av_player_construct(o, info);

  return o;
}

static void fr_av_player_dispose(SysObject* o) {
  FrAvPlayer *self = FR_AV_PLAYER(o);

  if(self->packet_decoder) {

    fr_decoder_stop(self->packet_decoder);
    sys_clear_pointer(&self->packet_decoder, _sys_object_unref);
  }

  if(self->video_decoder) {

    fr_decoder_stop(self->video_decoder);
    sys_clear_pointer(&self->video_decoder, _sys_object_unref);
  }

  if(self->audio_decoder) {

    fr_decoder_stop(self->audio_decoder);
    sys_clear_pointer(&self->audio_decoder, _sys_object_unref);
  }
  self->pkt_count = 0;

  sys_clear_pointer(&self->file, _sys_object_unref);
  sys_clear_pointer(&self->window, _sys_object_unref);
  sys_clear_pointer(&self->render, _sys_object_unref);

  SYS_OBJECT_CLASS(fr_av_player_parent_class)->dispose(o);
}

static void fr_av_player_class_init(FrAvPlayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrPlayerClass *pcls = FR_PLAYER_CLASS(cls);

  pcls->init = fr_av_player_init_i;
  pcls->process = fr_av_player_process_i;
  pcls->stop = fr_av_player_stop_i;

  ocls->dispose = fr_av_player_dispose;
}

void fr_av_player_init(FrAvPlayer* self) {
  self->seek_position = -1;
  self->max_packet = 8;
  self->min_packet = 4;
  self->region = NULL;
}
