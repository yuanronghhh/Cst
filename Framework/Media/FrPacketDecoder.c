#include <Framework/Media/FrPacketDecoder.h>
#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrMediaPipeline.h>

/* media packet decoder */
SYS_DEFINE_TYPE(FrPacketDecoder, fr_packet_decoder, FR_TYPE_DECODER);

static void cached_packet(FrPacketDecoder *self) {
  FrMediaPacket* mpkt;
  FrPacket *npkt;
  SysInt i = 0;
  FrDecoder *o = FR_DECODER(self);
  FrMediaPipeline *box = fr_decoder_get_user_data(o);

  sys_debug_N("%s", "cached it");
  do {
    mpkt = NULL;
    fr_media_file_read_packet(self->file, &mpkt);
    if(mpkt == NULL) {

      fr_media_pipeline_stop_player(box);
      break;
    }

    npkt = (FrPacket *)sys_object_dclone(mpkt);
    fr_decoder_push_packet(o, npkt);
    i++;
  } while (i < self->max_pkt);
}

SysInt fr_packet_decoder_decode_check_i(FrDecoder *o) {
  // FrPacketDecoder *self = FR_PACKET_DECODER(o);

  // if(len <= self->min_pkt) {

  //   cached_packet(self);
  // }

  return 0;
}

SysInt fr_packet_decoder_decode_it_i(FrDecoder *o, FrPacket *pkt) {
  SysInt sindex;
  FrDecoder *dec;
  FrMediaPacket *mpkt;

  FrMediaPipeline *box = fr_decoder_get_user_data(o);
  mpkt = FR_MEDIA_PACKET(pkt);

  sindex = fr_media_packet_get_stream_index(mpkt);

  dec = fr_media_pipeline_get_decoder(box, sindex);
  if(dec == NULL) {
    sys_warning_N("Not found media packet type: %s", sindex);
    return -1;
  }
  fr_decoder_push_packet(dec, pkt);

  return FR_DECODER_CLASS(fr_packet_decoder_parent_class)->decode_it(o, FR_PACKET(mpkt));
}

/* object api */
static void fr_packet_decoder_construct_i(
  FrDecoder* o,
  FrDecoderContext *info) {

  FR_DECODER_CLASS(fr_packet_decoder_parent_class)->construct(o, info);
}

static void packet_decoder_construct (FrPacketDecoder *self, 
    FrMediaFile *file) {
  FrDecoder *o = FR_DECODER(self);
  FrDecoderContext info = { .name = "packet_decoder" };
  fr_packet_decoder_construct_i(o, &info);

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
  self->max_pkt = 48;

}
