#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrMediaFrame.h>

SYS_DEFINE_TYPE(FrMediaStream, fr_media_stream, FR_TYPE_STREAM);

SysBool fr_media_stream_get_rational(FrMediaStream* self, FrRational* rt) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(rt != NULL, false);

  *rt = self->ctx->r_frame_rate;
  return true;
}

SysInt64 fr_media_stream_init_frame(
    FrMediaStream *self,
    FrMediaFrame *frame) {
  sys_return_val_if_fail(frame != NULL, AV_NOPTS_VALUE);
  sys_return_val_if_fail(self != NULL, AV_NOPTS_VALUE);

  SysUInt64 pts = fr_media_frame_get_pts(frame);

  switch(self->media_type) {
    case AVMEDIA_TYPE_VIDEO: {
      return av_rescale_q (frame->ctx->best_effort_timestamp,
          self->ctx->time_base,
          AV_TIME_BASE_Q);
    }
    case AVMEDIA_TYPE_AUDIO: {
      AVRational tb = (AVRational){1, frame->ctx->sample_rate};
      return av_rescale_q (pts, self->ctx->time_base, tb);
    }
    default:
      break;
  }

  return AV_NOPTS_VALUE;
}


SysBool fr_media_stream_is_media(FrMediaStream *self, FR_MEDIA_ENUM media_type) {
  return self->media_type == media_type;
}

FrMediaStream* fr_media_streams_get_by_index(
    FrMediaStream *streams[],
    SysUInt  n_streams,
    SysInt stream_index) {
  sys_return_val_if_fail(streams != NULL, NULL);
  sys_return_val_if_fail(stream_index >= 0, NULL);

  for (SysUInt i = 0; i < n_streams; i++) {
    if (streams[i]->ctx->index == stream_index) {
      return streams[i];
    }
  }

  return NULL;
}

FrMediaStream* fr_media_streams_get_by_media_type(
  FrMediaStream* streams[],
  SysUInt  n_streams,
  FR_MEDIA_ENUM media_type) {
  sys_return_val_if_fail(streams != NULL, NULL);
  sys_return_val_if_fail(media_type >= 0, NULL);
  sys_return_val_if_fail(media_type <= FR_MEDIA_PACKET, NULL);

  return streams[media_type];
}

void fr_media_stream_set_ctx(FrMediaStream *self, AVStream * ctx) {
  sys_return_if_fail(self != NULL);

  self->ctx = ctx;
}

AVStream * fr_media_stream_get_ctx(FrMediaStream *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->ctx;
}

/* object api */
static void fr_media_stream_construct(
    FrMediaStream *self,
    AVStream *stream,
    SysInt media_type) {

  self->ctx = stream;
  self->media_type = media_type;
}

FrMediaStream* fr_media_stream_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_STREAM, NULL);
}

FrMediaStream *fr_media_stream_new_I(
    AVStream *stream,
    SysInt media_type) {
  FrMediaStream *o = fr_media_stream_new();

  fr_media_stream_construct(o, stream, media_type);

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
