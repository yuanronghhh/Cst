#ifndef __FR_PACKET_H__
#define __FR_PACKET_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_PACKET (fr_packet_get_type())
#define FR_PACKET(o) ((FrPacket* )sys_object_cast_check(o, FR_TYPE_PACKET))
#define FR_PACKET_CLASS(o) ((FrPacketClass *)sys_class_cast_check(o, FR_TYPE_PACKET))
#define FR_PACKET_GET_CLASS(o) sys_instance_get_class(o, FrPacketClass)

struct _FrPacketClass {
  SysObjectClass parent;
};

struct _FrPacket {
  SysObject parent;

  /* <private> */
  SysInt serial;
};

SYS_API SysType fr_packet_get_type(void);
SYS_API FrPacket *fr_packet_new(void);

void fr_packet_set_serial(FrPacket *self, SysInt serial);
SysInt fr_packet_get_serial(FrPacket *self);
SysBool fr_packet_empty(FrPacket* self);

SYS_END_DECLS

#endif
