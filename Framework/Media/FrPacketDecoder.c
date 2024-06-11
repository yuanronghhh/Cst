#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrMediaPipeline.h>

SYS_DEFINE_TYPE(FrPacketDecoder, fr_packet_decoder, FR_TYPE_DECODER);

SysInt fr_packet_decoder_decode_it_i(FrDecoder *o, SysPointer user_data) {
  FrMediaPacket* mpkt;
  FrPacket* pkt;
  SysInt sindex;
  SysInt err;
  FrDecoder *dec;

  FrPacketDecoder *self = FR_PACKET_DECODER(o);
  FrMediaPipeline *box = user_data;

  mpkt = NULL;
  err = fr_media_file_read_packet(self->file, &mpkt);
  if(err < 0) { return err; }
  self->serial++;
  mpkt->parent.serial = self->serial;

  pkt = (FrPacket *)sys_object_dclone(mpkt);
  sindex = fr_media_packet_get_stream_index(mpkt);
  fr_packet_set_serial(pkt, self->serial);

  dec = fr_media_pipeline_get_decoder(box, sindex);
  if(dec == NULL) {
    sys_warning_N("Not found media packet type: %s", sindex);
    return -1;
  }

  if(!fr_decoder_push_packet(dec, pkt)) {
    sys_object_unref(pkt);

    return FR_MEDIA_ERROR_WAIT;
  }

  return FR_MEDIA_ERROR_AGAIN;
}

/* object api */
static void fr_packet_decoder_construct_i(
  FrDecoder* o,
  const SysChar* name) {

  FR_DECODER_CLASS(fr_packet_decoder_parent_class)->construct(o, name);
}

static void packet_decoder_construct (FrPacketDecoder *self, 
    FrMediaFile *file) {
  FrDecoder *o = FR_DECODER(self);
  FR_DECODER_CLASS(fr_packet_decoder_parent_class)->construct(o, "packet_decoder");

  self->file = sys_object_ref(file);
}

FrDecoder* fr_packet_decoder_new(void) {
  return sys_object_new(FR_TYPE_PACKET_DECODER, NULL);
}

FrDecoder *fr_packet_decoder_new_I(FrMediaFile *file) {
  FrDecoder *o = fr_packet_decoder_new();

  packet_decoder_construct((FrPacketDecoder *)o, file);

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

  dcls->decode_it = fr_packet_decoder_decode_it_i;
  dcls->construct = fr_packet_decoder_construct_i;

  ocls->dispose = fr_packet_decoder_dispose;
}

void fr_packet_decoder_init(FrPacketDecoder* self) {
}
