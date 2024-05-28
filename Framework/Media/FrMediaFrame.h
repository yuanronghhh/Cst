#ifndef __FR_MEDIA_FRAME_H__
#define __FR_MEDIA_FRAME_H__

#include <Framework/Media/FrPacket.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_FRAME (fr_media_frame_get_type())
#define FR_MEDIA_FRAME(o) ((FrMediaFrame* )sys_object_cast_check(o, FR_TYPE_MEDIA_FRAME))
#define FR_MEDIA_FRAME_CLASS(o) ((FrMediaFrameClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_FRAME))
#define FR_MEDIA_FRAME_GET_CLASS(o) sys_instance_get_class(o, FrMediaFrameClass)

struct _FrMediaFrameClass {
  FrPacketClass parent;
};

struct _FrMediaFrame {
  FrPacket parent;

  /* <private> */
  AVFrame *ctx;
  SysPointer user_data;
};

SYS_API SysType fr_media_frame_get_type(void);
SYS_API FrMediaFrame *fr_media_frame_new(void);

#define fr_media_frame_create(o) sys_object_create((o), FR_TYPE_MEDIA_FRAME)
SYS_API void fr_media_frame_set_ctx(FrMediaFrame *self, AVFrame * ctx);
SYS_API AVFrame * fr_media_frame_get_ctx(FrMediaFrame *self);

SYS_END_DECLS

#endif
