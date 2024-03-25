#include <Framework/Graph/FrFontContext.h>

SYS_DEFINE_TYPE(FrFontContext, fr_font_context, FR_TYPE_CONTEXT);


void fr_font_context_draw_text(FrFontContext* self, PangoLayout* layout, SysInt x, SysInt y) {
  cairo_t *cr = fr_context_get_cr(FR_CONTEXT(self));

  cairo_move_to(cr, x, y);
  pango_cairo_show_layout(cr, layout);
}

void fr_font_context_show_text(FrFontContext* self, PangoLayout *layout,
  SysInt x, SysInt y, SysInt m1, SysInt m0) {
  FrContext* ctx = FR_CONTEXT(self);

  fr_context_move_to(ctx, x + m1, y + m0);
  fr_context_show_layout(ctx, layout);
}

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
  FrFontContext *self = FR_FONT_CONTEXT(o);

  SYS_OBJECT_CLASS(fr_font_context_parent_class)->dispose(o);
}

static void fr_font_context_class_init(FrFontContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_font_context_dispose;
}

void fr_font_context_init(FrFontContext* self) {
}

