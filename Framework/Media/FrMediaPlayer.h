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
  FrMediaPipeline pipeline;

  SysBool use_hwaccel;
  SysInt64 seek_position;
  FR_MEDIA_STATE_ENUM state;
  SysDouble interval;
  FrIMediaRender *render;
};

SYS_API SysType fr_media_player_get_type(void);
SYS_API FrMediaPlayer *fr_media_player_new(void);

SYS_API FrMediaPlayer *fr_media_player_new_I(FrMediaFile *file);
void fr_media_player_set_render(FrMediaPlayer* self, FrIMediaRender *render);
SysInt fr_media_player_run(FrMediaPlayer* self);
void fr_media_player_play(FrMediaPlayer* self);
SysInt fr_media_player_render(FrMediaPlayer *self, FrIMediaRender *render, FrRegion *region);

void fr_media_player_set_state(FrMediaPlayer *self, FR_MEDIA_STATE_ENUM value);
FR_MEDIA_STATE_ENUM fr_media_player_get_state(FrMediaPlayer *self);

SYS_END_DECLS

#endif
