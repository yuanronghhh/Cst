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

static PipePass* pipe_pass_new_by_type(
    FrAvPlayer *pipe,
    FR_MEDIA_ENUM type,
    FrPacket *pkt) {
  sys_return_val_if_fail(pipe != NULL, NULL);
  PipePass *pass = sys_new0_N(PipePass, 1);

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
  FrMediaFrame *mframe = NULL;
  FrMediaPacket *nframe = NULL;

  err = fr_media_decoder_send_packet(mdec, mpkt);
  if(err < 0) { goto fail; }

  err = fr_media_decoder_try_decode_frame(mdec, &mframe);
  if(err < 0) { goto fail; }

  nframe = (FrMediaPacket *)sys_object_dclone(mframe);
  fr_media_decoder_write(mdec, nframe);
  sys_atomic_int_dec(&pass->pipe->pkt_count);

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
  FrAvPlayer *pipe = user_data;

  err = fr_decoder_decode(pipe->packet_decoder, &npkt);
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



static void process(FrAvPlayer* self) {
  FrDecoder *o = FR_DECODER(self->packet_decoder);
  if(fr_decoder_get_eof(o)) {
    fr_av_player_set_state(self, FR_JOB_STATE_STOP);
    return;
  }

  if (self->pkt_count > self->min_packet) {
    return;
  }
  sys_debug_N("wakeup %ld", self->pkt_count);

  for(int i = 0; i < self->max_packet; i++) {

    fr_decoder_run_async(o, process_packet, self);
    sys_atomic_int_inc(&self->pkt_count);
  }
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

  return decoder;
}

void av_player_get_video_size(FrAvPlayer *self, SysInt *width, SysInt *height) {
  sys_return_if_fail(self != NULL);

  FrVideoDecoder *video_decoder = FR_VIDEO_DECODER(self->video_decoder);
  if(video_decoder == NULL) { return; }

  fr_video_decoder_get_size(video_decoder, width, height);
}

static void av_player_run(FrAvPlayer *self, FrMediaFile *file) {
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

static void calc_video_delay (FrAvPlayer *self) {
  FrMediaStream* vs;
  FrRational rt = { .num = 2997, .den = 100 };

  vs = fr_media_file_stream_by_type(self->file, FR_MEDIA_VIDEO);
  fr_media_stream_get_rational(vs, &rt);
  self->delay = self->default_delay = (1 / (rt.num / (double) rt.den)) * 1.0e6;
}

static SysInt media_player_do(FrAvPlayer *self) {
  FrRegion* region;
  FrBound bound = { .width = 800, .height = 600 };

  if(self->state != FR_JOB_STATE_RUNNING) {
    return -1;
  }

  if(fr_media_file_has_video(self->file)) {
    calc_video_delay(self);
  }

  av_player_run(self, self->file);
  av_player_get_video_size(self, &bound.width, &bound.height);
  fr_window_set_size(self->window, bound.width, bound.height);

  region = fr_region_create_rectangle(&bound);

  while(self->state == FR_JOB_STATE_RUNNING) {
    process(self);

    fr_av_player_render(self, self->render, region);

    fr_delay(self->delay);
    fr_poll_events();
  }

  fr_region_destroy(region);

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

SysInt fr_av_player_run(FrAvPlayer* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return media_player_do(self);
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

  media_player_calc_diff(self);

  return FR_MEDIA_ERROR_EOF;
}

void fr_av_player_play(FrAvPlayer* self) {
  sys_return_if_fail(self != NULL);

  fr_media_file_play(self->file);
}

void fr_av_player_set_state(FrAvPlayer *self, FR_JOB_STATE_ENUM value) {
  sys_return_if_fail(self != NULL);

  self->state = value;
}

FR_JOB_STATE_ENUM fr_av_player_get_state(FrAvPlayer *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->state;
}

/* object api */
static void fr_av_player_construct(FrAvPlayer *self, FrAvPlayerContext *info) {
  self->file = sys_object_ref(info->file);
  self->window = sys_object_ref(info->window);
  self->render = sys_object_ref(info->render);
}

FrAvPlayer* fr_av_player_new(void) {
  return sys_object_new(FR_TYPE_AV_PLAYER, NULL);
}

FrAvPlayer *fr_av_player_new_I(FrAvPlayerContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->file != NULL, NULL);
  sys_return_val_if_fail(info->window != NULL, NULL);
  sys_return_val_if_fail(info->render != NULL, NULL);

  FrAvPlayer *o = fr_av_player_new();
  fr_av_player_construct(o, info);

  return o;
}

static void fr_av_player_dispose(SysObject* o) {
  FrAvPlayer *self = FR_AV_PLAYER(o);

  fr_decoder_stop(self->packet_decoder);
  sys_clear_pointer(&self->packet_decoder, _sys_object_unref);

  fr_decoder_stop(self->video_decoder);
  sys_clear_pointer(&self->video_decoder, _sys_object_unref);

  fr_decoder_stop(self->audio_decoder);
  sys_clear_pointer(&self->audio_decoder, _sys_object_unref);
  self->pkt_count = 0;

  sys_clear_pointer(&self->file, _sys_object_unref);
  sys_clear_pointer(&self->window, _sys_object_unref);
  sys_clear_pointer(&self->render, _sys_object_unref);

  SYS_OBJECT_CLASS(fr_av_player_parent_class)->dispose(o);
}

static void fr_av_player_class_init(FrAvPlayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_av_player_dispose;
}

void fr_av_player_init(FrAvPlayer* self) {
  self->state = FR_JOB_STATE_RUNNING;
  self->seek_position = -1;
  self->max_packet = 2;
  self->min_packet = 1;
}
