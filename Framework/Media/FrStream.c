#include <Framework/Media/FrStream.h>

SYS_DEFINE_TYPE(FrStream, fr_stream, SYS_TYPE_OBJECT);

/* object api */
FrStream* fr_stream_new(void) {
  return sys_object_new(FR_TYPE_STREAM, NULL);
}

static void fr_stream_dispose(SysObject* o) {
  FrStream *self = FR_STREAM(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_stream_parent_class)->dispose(o);
}

static void fr_stream_class_init(FrStreamClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_stream_dispose;
}

void fr_stream_init(FrStream* self) {
}
