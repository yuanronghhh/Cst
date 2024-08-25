#include <Framework/Media/FrAudioDecoder.h>
#include <Framework/Device/FrWindow.h>

SYS_DEFINE_TYPE(FrAudioDecoder, fr_audio_decoder, FR_TYPE_MEDIA_DECODER);

static SysInt fr_audio_decoder_open_i(FrDecoder* o) {

  return FR_DECODER_CLASS(fr_audio_decoder_parent_class)->open(o);
}

static SysInt fr_audio_decoder_decode_frame_i(FrMediaDecoder* o,
    FrMediaFrame **frame) {

  SysInt err = FR_MEDIA_DECODER_CLASS(fr_audio_decoder_parent_class)
    ->decode_frame(o, frame);

  return err;
}

/* object api */
FrMediaDecoder* fr_audio_decoder_new(void) {
  return sys_object_new(FR_TYPE_AUDIO_DECODER, NULL);
}

static void fr_audio_decoder_construct_i(FrMediaDecoder *o, FrAudioDecoderContext *info) {
  FrAudioDecoder *self = FR_AUDIO_DECODER(o);

  UNUSED(self);
}

FrMediaDecoder *fr_audio_decoder_new_I(FrAudioDecoderContext *info) {
  FrMediaDecoder *o = fr_audio_decoder_new();

  fr_audio_decoder_construct_i(o, info);

  return o;
}

static void fr_audio_decoder_dispose(SysObject* o) {


}

static void fr_audio_decoder_class_init(FrAudioDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrMediaDecoderClass *mcls = FR_MEDIA_DECODER_CLASS(cls);
  FrDecoderClass *dcls = FR_DECODER_CLASS(cls);

  mcls->decode_frame = fr_audio_decoder_decode_frame_i;
  dcls->open = fr_audio_decoder_open_i;

  ocls->dispose = fr_audio_decoder_dispose;
}

void fr_audio_decoder_init(FrAudioDecoder* self) {
  FrMediaDecoder *o = FR_MEDIA_DECODER(self);

  fr_media_decoder_set_frame_type(o, FR_TYPE_AUDIO_FRAME);
}
