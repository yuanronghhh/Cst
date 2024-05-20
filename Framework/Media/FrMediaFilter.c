#include <Framework/Media/FrMediaFilter.h>

SYS_DEFINE_TYPE(FrMediaFilter, fr_media_filter, SYS_TYPE_OBJECT);

/* object api */
static void fr_media_filter_construct(FrMediaFilter *self, AVFilter* ctx) {
  self->ctx = ctx;
}

FrMediaFilter* fr_media_filter_new(void) {
  return sys_object_new(FR_TYPE_MEDIA_FILTER, NULL);
}

FrMediaFilter *fr_media_filter_new_I(AVFilter *ctx) {
  FrMediaFilter *o = fr_media_filter_new();

  fr_media_filter_construct(o, ctx);

  return o;
}

static void fr_media_filter_dispose(SysObject* o) {
  FrMediaFilter *self = FR_MEDIA_FILTER(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_media_filter_parent_class)->dispose(o);
}

static void fr_media_filter_class_init(FrMediaFilterClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_media_filter_dispose;
}

void fr_media_filter_init(FrMediaFilter* self) {
}
