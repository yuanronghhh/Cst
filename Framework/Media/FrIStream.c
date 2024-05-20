#include <Framework/Media/FrIStream.h>

SYS_DEFINE_INTERFACE(FrIStream, fr_i_stream, SYS_TYPE_OBJECT);

/* object api */
void fr_i_stream_default_init(FrIStreamInterface* iface) {
}

SysBool fr_i_stream_read_packet (FrIStream *self, FrPacket **pkt) {
  sys_return_val_if_fail(self != NULL, false);

  return FR_I_STREAM_GET_IFACE(self)->read_packet(self, pkt);
}
