#include <Framework/Media/FrMediaPacket.h>

SYS_DEFINE_TYPE(FrMediaPacket, fr_media_packet, FR_TYPE_PACKET);

SysBool fr_media_packet_destroy_i(SysObject* o) {
  FrMediaPacket* self = FR_MEDIA_PACKET(o);
  if(self->ctx == NULL) { return false; }

  av_packet_free(&self->ctx);

  return SYS_OBJECT_CLASS(fr_media_packet_parent_class)->destroy(o);
}

SysInt fr_media_packet_get_stream_index(FrMediaPacket *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->ctx->stream_index;
}

SysObject *fr_media_packet_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysObject* n = SYS_OBJECT_CLASS(fr_media_packet_parent_class)->dclone(o);

  FrMediaPacket* nself = FR_MEDIA_PACKET(n);
  FrMediaPacket* oself = FR_MEDIA_PACKET(o);

  av_packet_ref(nself->ctx, oself->ctx);

  return n;
}

/* object api */
FrPacket* fr_media_packet_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_PACKET, NULL);
}

static void fr_media_packet_dispose(SysObject* o) {
  FrMediaPacket *self = FR_MEDIA_PACKET(o);

  av_packet_free(&self->ctx);

  SYS_OBJECT_CLASS(fr_media_packet_parent_class)->dispose(o);
}

static void fr_media_packet_class_init(FrMediaPacketClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dclone = fr_media_packet_dclone_i;
  ocls->destroy = fr_media_packet_destroy_i;
  ocls->dispose = fr_media_packet_dispose;
}

void fr_media_packet_init(FrMediaPacket* self) {
  self->ctx = av_packet_alloc();
}
