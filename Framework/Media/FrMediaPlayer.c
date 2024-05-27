#include <Framework/Media/FrMediaPlayer.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrFrameDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrIMediaRender.h>
#include <Framework/Media/FrVideoDecoder.h>
#include <Framework/Device/FrWindow.h>

SYS_DEFINE_TYPE(FrMediaPlayer, fr_media_player, SYS_TYPE_OBJECT);

void fr_media_player_wait(FrMediaPlayer *self) {
  fr_wait_events();
}

static SysInt process_task() {
}

static SysInt media_player_do(FrMediaPlayer *self) {
  FrBound bound = { .width = 800, .height = 600 };
  FrVideoDecoder* video_decoder;
  FrRegion* region;
  SysInt err = 0;

  if(!self->state & FR_MEDIA_STATE_RUNNING) {
    return -1;
  }
  fr_pipeline_run(&self->pipeline, self->file);

  video_decoder = FR_VIDEO_DECODER(self->pipeline.video_decoder);
  fr_video_decoder_get_size(video_decoder, &bound.width, &bound.height);
  region = fr_region_create_rectangle(&bound);

  while(self->state & FR_MEDIA_STATE_RUNNING) {
    fr_media_player_wait(self);

    err = process_task(self);
    if (err < 0) { goto done; }

    // err = process_packet(self);

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

void fr_media_player_get_frame(FrMediaPlayer *self, FrVideoFrame **frame) {
  *frame = sys_async_queue_try_pop(&(self->pipeline.image_queue));
}

SysInt fr_media_player_render(FrMediaPlayer *self, FrIMediaRender *render, FrRegion *region) {
  sys_return_val_if_fail(self != NULL, -1);
  FrVideoFrame *frame = NULL;
  FrIMediaRenderInterface *iface = FR_I_MEDIA_RENDER_GET_IFACE(render);

  fr_media_player_get_frame(self, &frame);
  if (frame == NULL) { return FR_MEDIA_ERROR_EOF; }

  // sys_debug_N("%d", frame->parent.parent.serial);
  iface->render_video(render, frame, region);
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

  fr_pipeline_create(&self->pipeline);
}
