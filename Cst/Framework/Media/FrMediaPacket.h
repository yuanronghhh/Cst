#ifndef __FR_MEDIA_PACKET_H__
#define __FR_MEDIA_PACKET_H__

#include <Framework/Media/FrPacket.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_PACKET (fr_media_packet_get_type())
#define FR_MEDIA_PACKET(o) ((FrMediaPacket* )sys_object_cast_check(o, FR_TYPE_MEDIA_PACKET))
#define FR_MEDIA_PACKET_CLASS(o) ((FrMediaPacketClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_PACKET))
#define FR_MEDIA_PACKET_GET_CLASS(o) sys_instance_get_class(o, FrMediaPacketClass)

struct _FrMediaPacketClass {
  FrPacketClass parent;
};

struct _FrMediaPacket {
  FrPacket parent;

  /* <private> */
  AVPacket *ctx;
};

SYS_API SysType fr_media_packet_get_type(void);
SYS_API FrPacket *fr_media_packet_new(void);

SysInt fr_media_packet_get_stream_index(FrMediaPacket *self);
#define fr_media_packet_create(o) sys_object_create(o, FR_TYPE_MEDIA_PACKET)

SYS_END_DECLS

#endif
