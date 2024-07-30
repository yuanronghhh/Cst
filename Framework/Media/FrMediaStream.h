#ifndef __FR_MEDIA_STREAM_H__
#define __FR_MEDIA_STREAM_H__

#include <Framework/Media/FrStream.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_STREAM (fr_media_stream_get_type())
#define FR_MEDIA_STREAM(o) ((FrMediaStream* )sys_object_cast_check(o, FR_TYPE_MEDIA_STREAM))
#define FR_MEDIA_STREAM_CLASS(o) ((FrMediaStreamClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_STREAM))
#define FR_MEDIA_STREAM_GET_CLASS(o) sys_instance_get_class(o, FrMediaStreamClass)

struct _FrMediaStreamClass {
  FrStreamClass parent;
};

struct _FrMediaStream {
  FrStream parent;

  /* <private> */

  /* AVStream * */
  SysPointer ctx;
};

struct _FrMediaStreamContext {
  SysType mediaType;

  /* AVStream * */
  SysPointer ctx;
};

SYS_API SysType fr_media_stream_get_type(void);
SYS_API FrMediaStream *fr_media_stream_new(void);

SYS_API FrMediaStream *fr_media_stream_new_I(FrMediaStreamContext *info);

FrMediaStream* fr_media_streams_get_by_media_type (
  FrMediaStream* streams[],
  SysUInt  n_streams,
  FR_MEDIA_ENUM media_type);

SysBool fr_media_stream_is_media(FrMediaStream *self, FR_MEDIA_ENUM media_type);

SYS_END_DECLS

#endif
