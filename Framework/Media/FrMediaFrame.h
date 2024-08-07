#ifndef __FR_MEDIA_FRAME_H__
#define __FR_MEDIA_FRAME_H__

#include <Framework/Media/FrMediaPacket.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_FRAME (fr_media_frame_get_type())
#define FR_MEDIA_FRAME(o) ((FrMediaFrame* )sys_object_cast_check(o, FR_TYPE_MEDIA_FRAME))
#define FR_MEDIA_FRAME_CLASS(o) ((FrMediaFrameClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_FRAME))
#define FR_MEDIA_FRAME_GET_CLASS(o) sys_instance_get_class(o, FrMediaFrameClass)

struct _FrMediaFrameClass {
  FrMediaPacketClass parent;

  /* init after receive_frame decode */
  void (*init_frame) (FrMediaFrame *self, FrMediaStream *stream);
};

struct _FrMediaFrame {
  FrMediaPacket parent;

  /* <private> */
  SysPointer ctx;
  SysPointer user_data;
  SysInt64 timestamp;
};

SYS_API SysType fr_media_frame_get_type(void);
SYS_API FrMediaFrame *fr_media_frame_new(void);

void fr_media_frame_init_frame(FrMediaFrame *self, FrMediaStream *stream) ;
#define fr_media_frame_create(o) sys_object_create((o), FR_TYPE_MEDIA_FRAME)

void fr_media_frame_set_timestamp(FrMediaFrame *self, SysInt64 timestamp);
SysInt64 fr_media_frame_get_timestamp(FrMediaFrame *self);

SYS_END_DECLS

#endif
