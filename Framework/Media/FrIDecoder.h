#ifndef __FR_I_DECODER__
#define __FR_I_DECODER__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_DECODER (fr_i_decoder_get_type())
#define FR_I_DECODER(o) ((FrIDecoder* )sys_object_cast_check(o, FR_TYPE_I_DECODER))
#define FR_I_DECODER_GET_IFACE(o) ((FrIDecoderInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_DECODER))

struct _FrIDecoderInterface {
  SysTypeInterface unowned;

  /* <private> */
};

SysType fr_i_decoder_get_type(void);


SYS_END_DECLS

#endif
