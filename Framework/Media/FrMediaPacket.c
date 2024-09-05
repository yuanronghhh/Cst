#include <Framework/Media/FrMediaPacket.h>

SYS_DEFINE_TYPE(FrMediaPacket, fr_media_packet, FR_TYPE_PACKET);

SysObject *fr_media_packet_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject* n = SYS_OBJECT_CLASS(fr_media_packet_parent_class)->dclone(o);

  FrMediaPacket* nself = FR_MEDIA_PACKET(n);
  FrMediaPacket* oself = FR_MEDIA_PACKET(o);

  fr_media_media_packet_ref(nself, oself);

  return n;
}

/* object api */

static void fr_media_packet_destroy(SysObject* o) {
  FrMediaPacket* self = FR_MEDIA_PACKET(o);

  fr_media_media_packet_unref((FrMediaPacket *)self);
}

static void fr_media_packet_dispose(SysObject* o) {
  fr_media_packet_destroy(o);

  SYS_OBJECT_CLASS(fr_media_packet_parent_class)->dispose(o);
}

FrPacket* fr_media_packet_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_PACKET, NULL);
}

static void fr_media_packet_unref_i(SysObject *o) {

  fr_media_media_packet_unref((FrMediaPacket *)o);
}

static void fr_media_packet_class_init(FrMediaPacketClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->unref = fr_media_packet_unref_i;
  ocls->destroy = fr_media_packet_destroy;

  ocls->dclone = fr_media_packet_dclone_i;
  ocls->dispose = fr_media_packet_dispose;
}

void fr_media_packet_init(FrMediaPacket* self) {

  fr_media_media_packet_create(self);
}
