#include <Framework/Media/FrPacket.h>

SYS_DEFINE_TYPE(FrPacket, fr_packet, SYS_TYPE_OBJECT);

void fr_packet_set_serial(FrPacket *self, SysInt serial) {
  sys_return_if_fail(self != NULL);

  self->serial = serial;
}

SysInt fr_packet_get_serial(FrPacket *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->serial;
}

SysBool fr_packet_empty(FrPacket* self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->serial == -1;
}

SysObject* fr_packet_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject* n;

  n = SYS_OBJECT_CLASS(fr_packet_parent_class)->dclone(o);

  FrPacket* nself = FR_PACKET(n);
  FrPacket* oself = FR_PACKET(o);

  nself->serial = oself->serial;

  return n;
}

/* object api */
FrPacket* fr_packet_new(void) {
  return sys_object_new(FR_TYPE_PACKET, NULL);
}

static void fr_packet_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_packet_parent_class)->dispose(o);
}

static void fr_packet_class_init(FrPacketClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dclone = fr_packet_dclone_i;
  ocls->dispose = fr_packet_dispose;
}

void fr_packet_init(FrPacket* self) {
  self->serial = -1;
}
