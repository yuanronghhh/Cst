#include <Framework/Media/FrVideoStream.h>

SYS_DEFINE_TYPE(FrVideoStream, fr_video_stream, FR_TYPE_MEDIA_STREAM);

/* object api */
static void fr_video_stream_construct_i(FrMediaStream *o,
    FrMediaStreamContext *info) {

  FR_MEDIA_STREAM_CLASS(fr_video_stream_parent_class)->construct(o, info);
}

FrMediaStream* fr_video_stream_new(void) {
  return sys_object_new(FR_TYPE_VIDEO_STREAM, NULL);
}

FrMediaStream *fr_video_stream_new_I(FrMediaStreamContext *info) {
  FrMediaStream *o = fr_video_stream_new();

  fr_video_stream_construct_i(o, info);

  return o;
}

static void fr_video_stream_dispose(SysObject* o) {
  FrVideoStream *self = FR_VIDEO_STREAM(o);

  UNUSED(self);


}

static void fr_video_stream_class_init(FrVideoStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrMediaStreamClass *mcls = FR_MEDIA_STREAM_CLASS(cls);

  mcls->construct = fr_video_stream_construct_i;
  ocls->dispose = fr_video_stream_dispose;
}

static void fr_video_stream_init(FrVideoStream* self) {
}
