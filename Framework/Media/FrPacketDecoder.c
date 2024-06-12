#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrMediaPipeline.h>

/* media packet decoder */
SYS_DEFINE_TYPE(FrPacketDecoder, fr_packet_decoder, FR_TYPE_DECODER);


static void cached_packet(FrPacketDecoder *self) {
  FrMediaPacket* mpkt = NULL;
  FrPacket *pkt;
  SysInt err;
  SysInt i = 0;
  FrDecoder *dec = FR_DECODER(self);

  do {
    err = fr_media_file_read_packet(self->file, &mpkt);
    if (err >= 0) {
      pkt = FR_PACKET(mpkt);

      fr_decoder_push_packet_unlock(dec, pkt);
      mpkt = NULL;
      i++;
    }
  } while (i < self->max_pkt && err != FR_MEDIA_ERROR_AGAIN);
}

SysInt fr_packet_decoder_decode_check_i(FrDecoder *o) {
  SysUInt len = fr_decoder_get_length(o);
  FrPacketDecoder *self = FR_PACKET_DECODER(o);

  if(len < self->min_pkt) {

    cached_packet(self);
    return FR_MEDIA_ERROR_WAIT;
  }

  return FR_MEDIA_ERROR_AGAIN;
}

SysInt fr_packet_decoder_decode_it_i(FrDecoder *o, FrPacket *pkt) {
  SysInt sindex;
  SysInt err;
  FrDecoder *dec;
  FrMediaPacket *mpkt;

  FrMediaPipeline *box = fr_decoder_get_user_data(o);

  mpkt = (FrMediaPacket *)sys_object_dclone(pkt);
  sindex = fr_media_packet_get_stream_index(mpkt);

  dec = fr_media_pipeline_get_decoder(box, sindex);
  if(dec == NULL) {
    sys_warning_N("Not found media packet type: %s", sindex);
    return -1;
  }
  err = fr_media_pipeline_push_packet(box, dec, pkt);

  FR_DECODER_CLASS(fr_packet_decoder_parent_class)->decode_it(o, pkt);

  return err;
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

  dcls->decode_check = fr_packet_decoder_decode_check_i;
  dcls->decode_it = fr_packet_decoder_decode_it_i;
  dcls->construct = fr_packet_decoder_construct_i;

  ocls->dispose = fr_packet_decoder_dispose;
}

void fr_packet_decoder_init(FrPacketDecoder* self) {
  self->min_pkt = 10;
  self->max_pkt = 100;
}
