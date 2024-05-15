#include <Framework/Media/FrMediaPlayer.h>
#include <Framework/Media/FrMediaPacket.h>
#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrFrameDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrIMediaRender.h>
#include <Framework/Media/FrVideoDecoder.h>

SYS_DEFINE_TYPE(FrMediaPlayer, fr_media_player, SYS_TYPE_OBJECT);

static SYS_INLINE SysBool player_should_pause(FrMediaPlayer *self) {
  if(self->paused == -1) { return false; }

  return self->paused == fr_media_file_get_seek_position(self->file);
}

static SYS_INLINE SysBool player_should_seek(FrMediaPlayer *self) {
  if(self->seek_position == -1) { return false; }

  return self->seek_position != fr_media_file_get_seek_position(self->file);
}

static SysInt media_player_do(FrMediaPlayer *self) {
  if(!self->running) {
    return -1;
  }

  fr_pipeline_run(&self->pipeline, self->file);
  return 0;

  while(self->running) {
    if (player_should_pause(self)) {
      if(fr_media_file_pause(self->file) < 0) {
        break;
      }

      self->paused = -1;
    } else {

      fr_media_player_play(self);
    }

    if (player_should_seek(self)) {
        if(fr_media_file_seek(self->file, self->seek_position) < 0) {
          break;
        }

        self->seek_position = -1;
    }
  }

  return 0;
}

SysInt fr_media_player_run(FrMediaPlayer* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return media_player_do(self);
}

void fr_media_player_get_frame(FrMediaPlayer *self, FrVideoFrame **frame) {
  *frame = sys_async_queue_try_pop(&(self->pipeline.image_queue));
}

SysInt fr_media_player_render(FrMediaPlayer *self, FrIMediaRender *render) {
  sys_return_val_if_fail(self != NULL, -1);
  FrVideoFrame *frame = NULL;
  FrIMediaRenderInterface *iface = FR_I_MEDIA_RENDER_GET_IFACE(render);

  fr_media_player_get_frame(self, &frame);
  if (frame == NULL) { return FR_MEDIA_STATE_EOF; }

  iface->render_video(render, frame);

  return 0;
}

void fr_media_player_play(FrMediaPlayer* self) {
  sys_return_if_fail(self != NULL);

  fr_media_file_play(self->file);
}

void fr_media_player_set_pause(FrMediaPlayer *self) {
  sys_return_if_fail(self != NULL);

  self->paused = fr_media_file_get_seek_position(self->file);
}

void fr_media_player_set_running(FrMediaPlayer *self, SysBool running) {
  sys_return_if_fail(self != NULL);

  sys_atomic_int_set(&self->running, running);
}

SysBool fr_media_player_get_running(FrMediaPlayer *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->running;
}

SysInt64 fr_media_player_get_pause(FrMediaPlayer *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->paused;
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
  self->running = true;
  self->paused = -1;
  self->seek_position = -1;

  fr_pipeline_create(&self->pipeline);
}
