#ifndef __FR_I_STREAM__
#define __FR_I_STREAM__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_I_STREAM (fr_i_stream_get_type())
#define FR_I_STREAM(o) ((FrIStream* )sys_object_cast_check(o, FR_TYPE_I_STREAM))
#define FR_I_STREAM_GET_IFACE(o) ((FrIStreamInterface *)SYS_TYPE_GET_INTERFACE(o, FR_TYPE_I_STREAM))


struct _FrIStreamInterface {
  SysTypeInterface parent;

  SysInt (*open) (FrIStream *stream);
  void (*close) (FrIStream *stream);
  SysInt (*read_packet) (FrIStream *stream, FrPacket **pkt);
};


SysType fr_i_stream_get_type(void);

SysBool fr_i_stream_read_packet (FrIStream *self, FrPacket **pkt);


SYS_END_DECLS

#endif
