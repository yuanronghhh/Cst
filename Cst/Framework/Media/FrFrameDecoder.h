#ifndef __FR_FRAME_DECODER_H__
#define __FR_FRAME_DECODER_H__

#include <Framework/Media/FrDecoder.h>

SYS_BEGIN_DECLS

#define FR_TYPE_FRAME_DECODER (fr_frame_decoder_get_type())
#define FR_FRAME_DECODER(o) ((FrFrameDecoder* )sys_object_cast_check(o, FR_TYPE_FRAME_DECODER))
#define FR_FRAME_DECODER_CLASS(o) ((FrFrameDecoderClass *)sys_class_cast_check(o, FR_TYPE_FRAME_DECODER))
#define FR_FRAME_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrFrameDecoderClass)

struct _FrFrameDecoderClass {
  FrDecoderClass parent;
};

struct _FrFrameDecoder {
  FrDecoder parent;

  /* <private> */
};

SYS_API SysType fr_frame_decoder_get_type(void);
SYS_API FrDecoder *fr_frame_decoder_new(void);

SYS_API FrDecoder *fr_frame_decoder_new_I(void);

SYS_END_DECLS

#endif
