#include <Framework/Media/FrFrameDecoder.h>

SYS_DEFINE_TYPE(FrFrameDecoder, fr_frame_decoder, FR_TYPE_DECODER);

/* object api */
static void fr_frame_decoder_construct_i(FrDecoder * o, FrDecoderContext *info) {

  FR_DECODER_CLASS(fr_frame_decoder_parent_class)->construct(o, info);
}

FrDecoder* fr_frame_decoder_new(void) {
  return sys_object_new(FR_TYPE_FRAME_DECODER, NULL);
}

FrDecoder *fr_frame_decoder_new_I(void) {
  FrDecoder *o = fr_frame_decoder_new();

  FrDecoderContext info = {.name = "frame_decoder"};
  fr_frame_decoder_construct_i(o, &info);

  return o;
}

static void fr_frame_decoder_dispose(SysObject* o) {


}

static void fr_frame_decoder_class_init(FrFrameDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrDecoderClass* dcls = FR_DECODER_CLASS(cls);

  dcls->construct = fr_frame_decoder_construct_i;
  ocls->dispose = fr_frame_decoder_dispose;
}

void fr_frame_decoder_init(FrFrameDecoder* self) {
}
