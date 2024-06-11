#ifndef __FR_VIDEO_FRAME_H__
#define __FR_VIDEO_FRAME_H__

#include <Framework/Media/FrMediaFrame.h>

SYS_BEGIN_DECLS

#define FR_TYPE_VIDEO_FRAME (fr_video_frame_get_type())
#define FR_VIDEO_FRAME(o) ((FrVideoFrame* )sys_object_cast_check(o, FR_TYPE_VIDEO_FRAME))
#define FR_VIDEO_FRAME_CLASS(o) ((FrVideoFrameClass *)sys_class_cast_check(o, FR_TYPE_VIDEO_FRAME))
#define FR_VIDEO_FRAME_GET_CLASS(o) sys_instance_get_class(o, FrVideoFrameClass)

struct _FrVideoFrameClass {
  FrMediaFrameClass parent;
};

struct _FrVideoFrame  {
  FrMediaFrame parent;

  /* <private> */
  SysInt width;
  SysInt height;
  FrWindow *window;
  FrBound *bound;
};

SYS_API SysType fr_video_frame_get_type(void);
SYS_API FrVideoFrame *fr_video_frame_new(void);
#define fr_video_frame_create(o) sys_object_create(o, FR_TYPE_VIDEO_FRAME)

SYS_API FrVideoFrame *fr_video_frame_new_I(void);

void fr_video_frame_init_out_size(FrVideoFrame* self);
void fr_video_frame_set_out_size(FrVideoFrame* self, SysInt width, SysInt height);
void fr_video_frame_get_out_size(FrVideoFrame* self, SysInt* width, SysInt* height);

void fr_video_frame_get_frame_data(FrVideoFrame *self,
  SysUInt8 **frame_data[], 
  SysInt *linesize[]);

SysBool fr_video_frame_scale(FrVideoFrame *self, FrImageScale *scale);

SYS_END_DECLS

#endif
