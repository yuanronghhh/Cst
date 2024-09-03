#include <Framework/Media/FrStream.h>

SYS_DEFINE_TYPE(FrStream, fr_stream, SYS_TYPE_OBJECT);

/* object api */
static void fr_stream_construct_i(FrStream *self, FrStreamContext *info) {
  self->io_type = info->io_type;
}

FrStream* fr_stream_new(void) {
  return sys_object_new(FR_TYPE_STREAM, NULL);
}

FrStream *fr_stream_new_I(FrStreamContext *info) {
  FrStream *o = fr_stream_new();

  fr_stream_construct_i(o, info);

  return o;
}

static void fr_stream_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_stream_parent_class)->dispose(o);
}

static void fr_stream_class_init(FrStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_stream_dispose;
}

void fr_stream_init(FrStream* self) {
}
