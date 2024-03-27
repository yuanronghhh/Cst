#include <Framework/Graph/FrFontContext.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrDraw.h>

SYS_DEFINE_TYPE(FrFontContext, fr_font_context, FR_TYPE_CONTEXT);


/* object api */
static void fr_font_context_construct(FrFontContext *self) {

}

FrFontContext* fr_font_context_new(void) {
  return sys_object_new(FR_TYPE_FONT_CONTEXT, NULL);
}

FrFontContext *fr_font_context_new_I(void) {
  FrFontContext *o = fr_font_context_new();

  fr_font_context_construct(o);

  return o;
}

static void fr_font_context_dispose(SysObject* o) {
  // FrFontContext *self = FR_FONT_CONTEXT(o);

  SYS_OBJECT_CLASS(fr_font_context_parent_class)->dispose(o);
}

static void fr_font_context_class_init(FrFontContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_font_context_dispose;
}

void fr_font_context_init(FrFontContext* self) {
}

