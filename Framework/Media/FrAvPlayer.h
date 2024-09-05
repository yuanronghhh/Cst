#ifndef __FR_AV_PLAYER_H__
#define __FR_AV_PLAYER_H__

#include <Framework/Media/FrPlayer.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AV_PLAYER (fr_av_player_get_type())
#define FR_AV_PLAYER(o) ((FrAvPlayer* )sys_object_cast_check(o, FR_TYPE_AV_PLAYER))
#define FR_AV_PLAYER_CLASS(o) ((FrAvPlayerClass *)sys_class_cast_check(o, FR_TYPE_AV_PLAYER))
#define FR_AV_PLAYER_GET_CLASS(o) sys_instance_get_class(o, FrAvPlayerClass)

struct _FrAvPlayerClass {
  FrPlayerClass parent;
};

struct _FrAvPlayer {
  FrPlayer parent;

  /* <private>*/
  FrMediaFile *file;
  FrWindow *window;

  FrDecoder* video_decoder;
  FrDecoder* audio_decoder;
  FrDecoder* subtitle_decoder;
  FrDecoder* packet_decoder;
  FrDevice *audio_device;
  FrRegion *region;
  SysInt max_packet;
  SysInt min_packet;
  SysInt pkt_count;

  SysBool use_hwaccel;
  SysInt64 seek_position;
  FrIMediaRender *video_render;
  FrIMediaRender *audio_render;
  SysInt64 base_tsp;
  SysInt64 vtsp;
  SysDouble default_delay;
  SysDouble delay;
};

struct _FrAvPlayerContext {
  FrMediaFile *file;
  FrWindow *window;
  FrIMediaRender *video_render;
  FrIMediaRender *audio_render;
};

SYS_API SysType fr_av_player_get_type(void);
SYS_API FrPlayer* fr_av_player_new(void);

SYS_API FrPlayer *fr_av_player_new_I(FrAvPlayerContext *info);
SYS_API void fr_av_player_play(FrAvPlayer* self);

void fr_av_player_set_window(FrAvPlayer *self, FrWindow * window);
FrWindow * fr_av_player_get_window(FrAvPlayer *self);

SYS_END_DECLS

#endif
