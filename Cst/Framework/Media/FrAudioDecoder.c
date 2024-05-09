#include <Framework/Media/FrAudioDecoder.h>
#include <Framework/Media/FrMedia.h>
#include <Framework/Media/FrPipeline.h>

SYS_DEFINE_TYPE(FrAudioDecoder, fr_audio_decoder, FR_TYPE_MEDIA_DECODER);

static SysInt fr_audio_decoder_decode_it_i(FrDecoder* o, SysPointer user_data) {
  SysInt err = FR_DECODER_CLASS(fr_audio_decoder_parent_class)->decode_it(o, user_data);
  if (err < 0) { return err; }
  // fr_decoder_push_packet(box->sample_decoder, box->frame);

  return err;
}

FrMediaFrame* fr_audio_decoder_get_frame_i (FrMediaDecoder* o) {
  FrAudioDecoder* self = FR_AUDIO_DECODER(o);

  return FR_MEDIA_FRAME(&self->frame);
}

/* object api */
FrDecoder* fr_audio_decoder_new(void) {
  return sys_object_new(FR_TYPE_AUDIO_DECODER, NULL);
}

static void fr_audio_decoder_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_audio_decoder_parent_class)->dispose(o);
}

static void fr_audio_decoder_class_init(FrAudioDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDecoderClass *dcls = FR_DECODER_CLASS(cls);
  FrMediaDecoderClass *mcls = FR_MEDIA_DECODER_CLASS(cls);

  dcls->decode_it = fr_audio_decoder_decode_it_i;
  mcls->get_frame = fr_audio_decoder_get_frame_i;

  ocls->dispose = fr_audio_decoder_dispose;
}

void fr_audio_decoder_init(FrAudioDecoder* self) {

  fr_audio_frame_create(&self->frame);
}
