#include <Framework/Media/FrMediaPlayer.h>
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

SYS_DEFINE_TYPE(FrMediaPlayer, fr_media_player, SYS_TYPE_OBJECT);

void fr_media_player_wait(FrMediaPlayer *self) {
  fr_wait_events_timeout(self->interval);
}

static SysInt process(FrMediaPlayer* self) {

  return FR_MEDIA_STATE_RUNNING;
}

static SysInt media_player_do(FrMediaPlayer *self) {
  FrRegion* region;
  SysInt state;
  FrMediaStream* vs;
  FrRational rt = { .num = 2997, .den = 100 };
  FrBound bound = { .width = 800, .height = 600 };

  if(!(self->state & FR_MEDIA_STATE_RUNNING)) {
    return -1;
  }

  // get interval
  vs = fr_media_file_stream_by_type(self->file, FR_MEDIA_VIDEO);
  fr_media_stream_get_rational(vs, &rt);
  self->interval = (1 / (SysDouble)rt.num / rt.den);

  fr_media_pipeline_run(&self->pipeline, self->file);
  fr_media_pipeline_get_video_size(&self->pipeline, &bound.width, &bound.height);

  region = fr_region_create_rectangle(&bound);

  while(self->state & FR_MEDIA_STATE_RUNNING) {
    fr_media_player_wait(self);

    state = process(self);
    if (state & FR_MEDIA_STATE_STOP) { goto done; }

    fr_media_player_render(self, self->render, region);
  }

done:
  fr_region_destroy(region);

  return 0;
}

void fr_media_player_set_render(FrMediaPlayer* self, FrIMediaRender *render) {
  sys_return_if_fail(self != NULL);

  self->render = render;
}

SysInt fr_media_player_run(FrMediaPlayer* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return media_player_do(self);
}

SysInt fr_media_player_render(FrMediaPlayer *self, FrIMediaRender *render, FrRegion *region) {
  sys_return_val_if_fail(self != NULL, -1);
  FrMediaFrame *frame = NULL;
  FrVideoFrame *vframe;
  FrIMediaRenderInterface *iface = FR_I_MEDIA_RENDER_GET_IFACE(render);

  frame = fr_media_pipeline_get_image_frame(&self->pipeline);
  if (frame == NULL) { return FR_MEDIA_ERROR_EOF; }
  vframe = FR_VIDEO_FRAME(frame);

  // sys_debug_N("%d", frame->parent.serial);
  iface->render_video(render, vframe, region);
  sys_object_unref(frame);

  return 0;
}

void fr_media_player_play(FrMediaPlayer* self) {
  sys_return_if_fail(self != NULL);

  fr_media_file_play(self->file);
}

void fr_media_player_set_state(FrMediaPlayer *self, SysInt field, SysBool flag) {
  sys_return_if_fail(self != NULL);

  (flag ? bit_true(self->state, field) : bit_false(self->state, field));
}

SysBool fr_media_player_get_state(FrMediaPlayer *self, SysInt field) {
  sys_return_val_if_fail(self != NULL, false);

  return self->state & field;
}

/* object api */
static void fr_media_player_construct(FrMediaPlayer *self, FrMediaFile *file) {
  self->file = sys_object_ref(file);
}

FrMediaPlayer* fr_media_player_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_PLAYER, NULL);
}

FrMediaPlayer *fr_media_player_new_I(FrMediaFile *file) {
  FrMediaPlayer *o = fr_media_player_new();

  fr_media_player_construct(o, file);

  return o;
}

static void fr_media_player_dispose(SysObject* o) {
  FrMediaPlayer *self = FR_MEDIA_PLAYER(o);

  sys_object_destroy(&self->pipeline);
  sys_clear_pointer(&self->file, _sys_object_unref);

  SYS_OBJECT_CLASS(fr_media_player_parent_class)->dispose(o);
}

static void fr_media_player_class_init(FrMediaPlayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_media_player_dispose;
}

void fr_media_player_init(FrMediaPlayer* self) {
  self->state = FR_MEDIA_STATE_RUNNING;
  self->seek_position = -1;

  fr_media_pipeline_create(&self->pipeline);
}
