#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrMediaFrame.h>

SYS_DEFINE_TYPE(FrMediaStream, fr_media_stream, FR_TYPE_STREAM);

FrMediaStream* fr_media_streams_get_by_media_type(
  FrMediaStream* streams[],
  SysUInt  n_streams,
  FR_MEDIA_ENUM media_type) {
  sys_return_val_if_fail(streams != NULL, NULL);
  sys_return_val_if_fail(media_type >= 0, NULL);
  sys_return_val_if_fail(media_type <= FR_MEDIA_PACKET, NULL);

  return streams[media_type];
}

/* object api */
static void fr_media_stream_construct(
    FrMediaStream *self,
    FrMediaStreamContext *info) {

  fr_media_stream_create(self, info);
}

FrMediaStream* fr_media_stream_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_STREAM, NULL);
}

FrMediaStream *fr_media_stream_new_I(FrMediaStreamContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->mediaType, NULL);

  FrMediaStream *o = sys_object_new(info->mediaType, NULL);
  sys_return_val_if_fail(o != NULL, NULL);

  fr_media_stream_construct(o, info);

  return o;
}

static void fr_media_stream_dispose(SysObject* o) {
  // FrMediaStream *self = FR_MEDIA_STREAM(o);

  SYS_OBJECT_CLASS(fr_media_stream_parent_class)->dispose(o);
}

static void fr_media_stream_class_init(FrMediaStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_media_stream_dispose;
}

void fr_media_stream_init(FrMediaStream* self) {
}
