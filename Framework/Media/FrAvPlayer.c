#include <Framework/Media/FrAvPlayer.h>

#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrFrameDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrIMediaRender.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Media/FrMediaPipeline.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Device/FrWindow.h>
#include <Framework/Media/FrAudioFrame.h>
#include <Framework/Media/FrAvRender.h>

SYS_DEFINE_TYPE(FrAvPlayer, fr_av_player, FR_TYPE_PLAYER);

static void process(FrAvPlayer* self) {

  fr_media_pipeline_wakeup_source(&self->pipeline);
}

static SysInt media_player_do(FrAvPlayer *self) {
  FrRegion* region;
  FrMediaStream* vs;
  FrRational rt = { .num = 2997, .den = 100 };
  FrBound bound = { .width = 800, .height = 600 };

  if(self->state != FR_JOB_STATE_RUNNING) {
    return -1;
  }

  // get interval
  vs = fr_media_file_stream_by_type(self->file, FR_MEDIA_VIDEO);
  fr_media_stream_get_rational(vs, &rt);
  self->delay = self->default_delay = (1.0 / rt.num / (double) rt.den) * 1.0e9;

  fr_media_pipeline_run(&self->pipeline, self->file);
  fr_media_pipeline_get_video_size(&self->pipeline, &bound.width, &bound.height);
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
  SysInt64 diff = 0;

  if(fr_media_file_has_audio(self->file)) {

    if(fr_media_file_has_video(self->file)) {

       diff = self->vtsp - self->base_tsp;
    }
  } else if(fr_media_file_has_video(self->file)) {

  } else {
  }

  self->delay = diff <= 0 ? self->default_delay : diff / 1.0e9;
}

SysInt fr_av_player_render(FrAvPlayer *self,
    FrAvRender *render,
    FrRegion *region) {
  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(render != NULL, -1);
  sys_return_val_if_fail(region != NULL, -1);

  FrMediaFrame *frame = NULL;
  FrVideoFrame *vframe = NULL;

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

  frame = fr_media_pipeline_get_image_frame(&self->pipeline);
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

  sys_object_destroy(&self->pipeline);
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

  fr_media_pipeline_create(&self->pipeline);
  self->pipeline.player = self;
}
