#ifndef __FR_VIDEO_DECODER_H__
#define __FR_VIDEO_DECODER_H__

#include <Framework/Media/FrMediaDecoder.h>
#include <Framework/Media/FrVideoFrame.h>
#include <Framework/Media/FrImageScale.h>

SYS_BEGIN_DECLS

#define FR_TYPE_VIDEO_DECODER (fr_video_decoder_get_type())
#define FR_VIDEO_DECODER(o) ((FrVideoDecoder* )sys_object_cast_check(o, FR_TYPE_VIDEO_DECODER))
#define FR_VIDEO_DECODER_CLASS(o) ((FrVideoDecoderClass *)sys_class_cast_check(o, FR_TYPE_VIDEO_DECODER))
#define FR_VIDEO_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrVideoDecoderClass)

struct _FrVideoDecoderClass {
  FrMediaDecoderClass parent;
};

struct _FrVideoDecoder {
  FrMediaDecoder parent;

  /* <private> */
  FrVideoFrame frame;
  FrImageScale scale;
};

SYS_API SysType fr_video_decoder_get_type(void);
SYS_API FrDecoder *fr_video_decoder_new(void);
SYS_API void fr_video_decoder_resize(FrVideoDecoder *self, SysInt width, SysInt height);

SYS_END_DECLS

#endif
