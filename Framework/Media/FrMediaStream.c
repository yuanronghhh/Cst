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

void fr_media_stream_construct(
    FrMediaStream *o,
    FrMediaStreamContext *info) {
  sys_return_if_fail(o != NULL);

  FrMediaStreamClass* cls = FR_MEDIA_STREAM_GET_CLASS(o);
  sys_return_if_fail(cls->construct);

  cls->construct(o, info);
}

/* object api */
static void fr_media_stream_construct_i(
    FrMediaStream *self,
    FrMediaStreamContext *info) {

  fr_media_stream_create(self, info);
}

FrMediaStream* fr_media_stream_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_STREAM, NULL);
}

FrMediaStream *fr_media_stream_new_I(FrMediaStreamContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);

  FrMediaStream *o = fr_media_stream_new();
  sys_return_val_if_fail(o != NULL, NULL);

  fr_media_stream_construct_i(o, info);

  return o;
}

static void fr_media_stream_dispose(SysObject* o) {
  FrMediaStream *self = FR_MEDIA_STREAM(o);

  fr_media_stream_free(self);

  SYS_OBJECT_CLASS(fr_media_stream_parent_class)->dispose(o);
}

static void fr_media_stream_class_init(FrMediaStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_media_stream_construct_i;
  ocls->dispose = fr_media_stream_dispose;
}

void fr_media_stream_init(FrMediaStream* self) {

}
