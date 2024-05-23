#include <Framework/Media/FrAudioDecoder.h>
#include <Framework/Media/FrPipeline.h>

SYS_DEFINE_TYPE(FrAudioDecoder, fr_audio_decoder, FR_TYPE_MEDIA_DECODER);

static SysInt fr_audio_decoder_decode_frame_i(FrMediaDecoder* o, FrMediaFrame *frame) {
  FrDecoder *d = FR_DECODER(o);
  FrPipeline *box = fr_decoder_get_user_data(d);

  // sys_async_queue_push(&box->sample_queue, frame);

  return 0;
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
  FrMediaDecoderClass *mcls = FR_MEDIA_DECODER_CLASS(cls);

  mcls->decode_frame = fr_audio_decoder_decode_frame_i;
  mcls->get_frame = fr_audio_decoder_get_frame_i;

  ocls->dispose = fr_audio_decoder_dispose;
}

void fr_audio_decoder_init(FrAudioDecoder* self) {

  fr_audio_frame_create(&self->frame);
}
