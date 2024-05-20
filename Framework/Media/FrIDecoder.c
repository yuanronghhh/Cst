#include <Framework/Media/FrIDecoder.h>

SYS_DEFINE_INTERFACE(FrIDecoder, fr_i_decoder, SYS_TYPE_OBJECT);

/* object api */
void fr_i_decoder_default_init(FrIDecoderInterface* iface) {
}

FrPacket * fr_i_decoder_get_packet_ptr (FrIDecoder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return FR_I_DECODER_GET_IFACE(self)->get_packet_ptr(self);
}
