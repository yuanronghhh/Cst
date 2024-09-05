#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrMediaStream.h>

/* media packet decoder */
SYS_DEFINE_TYPE(FrPacketDecoder, fr_packet_decoder, FR_TYPE_DECODER);

SysInt fr_packet_decoder_decode_i(FrDecoder *o, FrPacket **npkt) {
  FrPacketDecoder *self;

  self = FR_PACKET_DECODER(o);

  fr_media_file_read_packet(self->file, (FrMediaPacket **)npkt);
  if(*npkt == NULL) {

    fr_decoder_set_eof(o, true);
    return -1;
  }

  return FR_MEDIA_ERROR_SUCCESS;
}

/* object api */
static void fr_packet_decoder_construct_i(
  FrDecoder* o,
  FrDecoderContext *info) {

  FR_DECODER_CLASS(fr_packet_decoder_parent_class)->construct(o, info);
}

static void fr_packet_decoder_construct (FrDecoder *o,
    FrMediaFile *file) {
  FrPacketDecoder *self = FR_PACKET_DECODER(o);

  FrDecoderContext info = { .name = "packet_decoder" };
  fr_packet_decoder_construct_i(o, &info);

  self->file = sys_object_ref(file);
}

FrDecoder* fr_packet_decoder_new(void) {
  return sys_object_new(FR_TYPE_PACKET_DECODER, NULL);
}

FrDecoder *fr_packet_decoder_new_I(FrMediaFile *file) {
  FrDecoder *o = fr_packet_decoder_new();

  fr_packet_decoder_construct(o, file);

  return o;
}

static void fr_packet_decoder_dispose(SysObject* o) {
  FrPacketDecoder *self = FR_PACKET_DECODER(o);

  sys_clear_pointer(&self->file, _sys_object_unref);

  SYS_OBJECT_CLASS(fr_packet_decoder_parent_class)->dispose(o);
}

static void fr_packet_decoder_class_init(FrPacketDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDecoderClass *dcls = FR_DECODER_CLASS(cls);

  dcls->construct = fr_packet_decoder_construct_i;
  dcls->decode = fr_packet_decoder_decode_i;

  ocls->dispose = fr_packet_decoder_dispose;
}

void fr_packet_decoder_init(FrPacketDecoder* self) {
}
