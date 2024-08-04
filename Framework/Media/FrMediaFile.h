#ifndef __FR_MEDIA_FILE_H__
#define __FR_MEDIA_FILE_H__

#include <Framework/Media/FrStream.h>
#include <Framework/Media/FrMediaPacket.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_FILE (fr_media_file_get_type())
#define FR_MEDIA_FILE(o) ((FrMediaFile* )sys_object_cast_check(o, FR_TYPE_MEDIA_FILE))
#define FR_MEDIA_FILE_CLASS(o) ((FrMediaFileClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_FILE))
#define FR_MEDIA_FILE_GET_CLASS(o) sys_instance_get_class(o, FrMediaFileClass)

struct _FrMediaFileClass {
  FrStreamClass parent;
};

struct _FrMediaFile {
  FrStream parent;

  /* <private> */
  AVFormatContext *ctx;
  SysBool is_realtime;

  FrMediaStream **streams;
  SysUInt n_streams;
  SysInt show_mode;

  struct {
    SysInt seek_flags;
    SysInt64 seek_position;
    SysInt64 seek_rel;
  } seek;

  FrMediaPacket pkt;
  SysInt serial;
};

SYS_API SysType fr_media_file_get_type(void);
SYS_API FrMediaFile *fr_media_file_new(void);

SYS_API FrMediaFile *fr_media_file_new_I(const SysChar *filename);
SYS_API SysInt fr_media_file_read_packet(FrMediaFile *self, FrMediaPacket **npkt);

SYS_API FrMediaStream* fr_media_file_stream_by_type(
    FrMediaFile *self,
    FR_MEDIA_ENUM mediaType);

SYS_API SysInt fr_media_file_pause(FrMediaFile* self);
SYS_API SysInt fr_media_file_play(FrMediaFile* self);
SYS_API const SysChar *fr_media_file_get_url(FrMediaFile *self);
SYS_API SysInt fr_media_file_seek(FrMediaFile *self, SysInt64 seek_target);

SysUInt fr_media_file_stream_count(FrMediaFile* self);
SysBool fr_media_file_has_video(FrMediaFile* self);
SysBool fr_media_file_has_audio(FrMediaFile* self);
SysBool fr_media_file_has_subtitle(FrMediaFile* self);

SysInt64 fr_media_file_get_seek_position(FrMediaFile *self);

SYS_END_DECLS

#endif
