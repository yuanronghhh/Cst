#ifndef __FR_MEDIA_PLAYER_H__
#define __FR_MEDIA_PLAYER_H__

#include <Framework/Media/FrPlayer.h>
#include <Framework/Media/FrMediaPipeline.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_PLAYER (fr_media_player_get_type())
#define FR_MEDIA_PLAYER(o) ((FrMediaPlayer* )sys_object_cast_check(o, FR_TYPE_MEDIA_PLAYER))
#define FR_MEDIA_PLAYER_CLASS(o) ((FrMediaPlayerClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_PLAYER))
#define FR_MEDIA_PLAYER_GET_CLASS(o) sys_instance_get_class(o, FrMediaPlayerClass)

struct _FrMediaPlayerClass {
  FrPlayerClass parent;
};

struct _FrMediaPlayer {
  FrPlayer parent;

  /* <private>*/
  FrMediaFile *file;
  FrWindow *window;
  FrMediaPipeline pipeline;

  SysBool use_hwaccel;
  SysInt64 seek_position;
  FR_JOB_STATE_ENUM state;
  SysDouble delay;
  SysDouble default_delay;
  FrIMediaRender *render;
  SysInt64 audio_pts;
  SysInt64 video_pts;
};

struct _FrMediaPlayerContext {
  FrMediaFile *file;
  FrWindow *window;
  FrIMediaRender *render;
};

SYS_API SysType fr_media_player_get_type(void);
SYS_API FrMediaPlayer *fr_media_player_new(void);

SYS_API FrMediaPlayer *fr_media_player_new_I(FrMediaPlayerContext *info);
SYS_API void fr_media_player_set_render(FrMediaPlayer* self, FrIMediaRender *render);
SYS_API SysInt fr_media_player_run(FrMediaPlayer* self);
SYS_API void fr_media_player_play(FrMediaPlayer* self);
SYS_API SysInt fr_media_player_render(FrMediaPlayer *self, FrIMediaRender *render, FrRegion *region);

SYS_API void fr_media_player_set_state(FrMediaPlayer *self, FR_JOB_STATE_ENUM value);
SYS_API FR_JOB_STATE_ENUM fr_media_player_get_state(FrMediaPlayer *self);

void fr_media_player_set_window(FrMediaPlayer *self, FrWindow * window);
FrWindow * fr_media_player_get_window(FrMediaPlayer *self);

SYS_END_DECLS

#endif
