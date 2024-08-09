#include <Framework/Media/FrMediaFile.h>
#include <Framework/Media/FrIStream.h>
#include <Framework/Media/FrPacket.h>
#include <Framework/Media/FrMediaStream.h>
#include <Framework/Media/FrMediaPacket.h>

typedef enum _SHOW_MODE_ENUM {
  SHOW_MODE_NONE = -1,
  SHOW_MODE_VIDEO = 0,
  SHOW_MODE_WAVES,
  SHOW_MODE_RDFT,
  SHOW_MODE_NB
} SHOW_MODE_ENUM;

static void i_stream_imp(FrIStreamInterface *iface);

SYS_DEFINE_WITH_CODE(FrMediaFile, fr_media_file, FR_TYPE_MEDIA_STREAM,
    SYS_IMPLEMENT_INTERFACE(FR_TYPE_I_STREAM, i_stream_imp));

SysInt64 fr_media_file_get_seek_position(FrMediaFile *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->seek.seek_position;
}

SysInt fr_media_file_seek(FrMediaFile *self, SysInt64 seek_target) {
  sys_return_val_if_fail(self != NULL, -1);

  SysInt64 seek_min;
  SysInt64 seek_max;
  SysInt err;

  seek_min = self->seek.seek_rel > 0 ?
    seek_target - self->seek.seek_rel + 2: INT64_MIN;

  seek_max = self->seek.seek_rel < 0 ?
    seek_target - self->seek.seek_rel - 2: INT64_MAX;

  err = avformat_seek_file(self->ctx,
      -1,
      seek_min,
      seek_target,
      seek_max,
      self->seek.seek_flags);

  return err;
}

SysInt fr_media_file_pause(FrMediaFile* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return av_read_pause(self->ctx);
}

SysInt fr_media_file_play(FrMediaFile* self) {
  sys_return_val_if_fail(self != NULL, -1);

  return av_read_play(self->ctx);
}

SysInt media_file_read_packet_i(FrIStream *o, FrPacket **pkt) {
  FrMediaFile *self = FR_MEDIA_FILE(o);
  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(*pkt == NULL, -1);

  SysInt err;
  FrPacket *npkt = FR_PACKET(&self->mpkt);

  err = fr_media_media_file_read_packet(self, &self->mpkt);
  if(err < 0) { return err; }

  self->serial++;
  fr_packet_set_serial(npkt, self->serial);
  *pkt = npkt;

  return err;
}

SysInt fr_media_file_read_packet(FrMediaFile *self, FrMediaPacket **npkt) {
  sys_return_val_if_fail(self != NULL, -1);
  FrIStream *sm = FR_I_STREAM(self);

  return media_file_read_packet_i(sm, (FrPacket **)npkt);
}

const SysChar *fr_media_file_get_url(FrMediaFile *self) {
  return self->ctx->url;
}

SysUInt fr_media_file_stream_count(FrMediaFile* self) {

  return self->n_streams;
}

SysBool fr_media_file_has_video(FrMediaFile* self) {

  return self->streams[FR_MEDIA_VIDEO] != NULL;
}

SysBool fr_media_file_has_audio(FrMediaFile* self) {

  return self->streams[FR_MEDIA_AUDIO] != NULL;
}

SysBool fr_media_file_has_subtitle(FrMediaFile* self) {

  return self->streams[FR_MEDIA_SUBTITLE] != NULL;
}

FrMediaStream* fr_media_file_stream_by_type(
  FrMediaFile* self,
  FR_MEDIA_ENUM mediaType) {
  sys_return_val_if_fail(self != NULL, NULL);

  return fr_media_streams_get_by_media_type(self->streams, self->n_streams, mediaType);
}

void fr_media_file_find_audio_device_info(FrMediaFile* self,
    FrAudioDeviceContext *info) {
  FrMediaStream* astream = fr_media_file_stream_by_type(self, FR_MEDIA_AUDIO);
}

static void i_stream_imp(FrIStreamInterface *iface) {
  iface->read_packet = media_file_read_packet_i;
}

/* object api */
static void fr_media_file_construct(FrMediaFile *self, const SysChar *filename) {

  fr_media_file_create(self, filename);
}

FrMediaFile* fr_media_file_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_FILE, NULL);
}

FrMediaFile *fr_media_file_new_I(const SysChar *filename) {
  sys_return_val_if_fail(filename != NULL, NULL);

  if(!sys_path_exists(filename)) {
    sys_warning_N("file not exist: %s", filename);
    return NULL;
  }

  FrMediaFile *o = fr_media_file_new();

  fr_media_file_construct(o, filename);

  return o;
}

static void fr_media_file_dispose(SysObject* o) {
  FrMediaFile *self = FR_MEDIA_FILE(o);

  for (SysUInt i = 0; i < self->n_streams; i++) {

    sys_clear_pointer(&self->streams[i], _sys_object_unref);
  }
  self->n_streams = 0;
  sys_clear_pointer(&self->streams, sys_free);

  sys_assert(self->ctx != NULL);
  avformat_close_input(&self->ctx);
  sys_object_destroy(&self->mpkt);

  SYS_OBJECT_CLASS(fr_media_file_parent_class)->dispose(o);
}

static void fr_media_file_class_init(FrMediaFileClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_media_file_dispose;
}

void fr_media_file_init(FrMediaFile* self) {
  self->show_mode = SHOW_MODE_VIDEO;
  self->serial = -1;
  fr_media_packet_create(&self->mpkt);
}
