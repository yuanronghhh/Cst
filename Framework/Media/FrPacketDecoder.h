#ifndef __FR_PACKET_DECODER_H__
#define __FR_PACKET_DECODER_H__

#include <Framework/Media/FrDecoder.h>

SYS_BEGIN_DECLS

#define FR_TYPE_PACKET_DECODER (fr_packet_decoder_get_type())
#define FR_PACKET_DECODER(o) ((FrPacketDecoder* )sys_object_cast_check(o, FR_TYPE_PACKET_DECODER))
#define FR_PACKET_DECODER_CLASS(o) ((FrPacketDecoderClass *)sys_class_cast_check(o, FR_TYPE_PACKET_DECODER))
#define FR_PACKET_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrPacketDecoderClass)

struct _FrPacketDecoderClass {
  FrDecoderClass parent;
};

struct _FrPacketDecoder {
  FrDecoder parent;

  /* <private> */
  FrMediaFile *file;
  SysUInt min_pkt;
  SysUInt max_pkt;
};

SysType fr_packet_decoder_get_type(void);
FrDecoder *fr_packet_decoder_new(void);

FrDecoder *fr_packet_decoder_new_I(FrMediaFile *file);

SYS_END_DECLS

#endif
