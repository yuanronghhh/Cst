#ifndef __FR_VIDEO_STREAM_H__
#define __FR_VIDEO_STREAM_H__

#include <Framework/Media/FrMediaStream.h>

SYS_BEGIN_DECLS

#define FR_TYPE_VIDEO_STREAM (fr_video_stream_get_type())
#define FR_VIDEO_STREAM(o) ((FrVideoStream* )sys_object_cast_check(o, FR_TYPE_VIDEO_STREAM))
#define FR_VIDEO_STREAM_CLASS(o) ((FrVideoStreamClass *)sys_class_cast_check(o, FR_TYPE_VIDEO_STREAM))
#define FR_VIDEO_STREAM_GET_CLASS(o) sys_instance_get_class(o, FrVideoStreamClass)

struct _FrVideoStreamClass {
  FrMediaStreamClass parent;
};

struct _FrVideoStream {
  FrMediaStream parent;

  /* <private> */
  SysPointer ctx;
};

struct _FrVideoStreamContext {
  SysInt format;
  SysInt sample;
  SysInt channels;
};

SYS_API SysType fr_video_stream_get_type(void);
SYS_API FrMediaStream *fr_video_stream_new(void);

SYS_API FrMediaStream *fr_video_stream_new_I(FrVideoStreamContext *info);

SYS_END_DECLS

#endif
