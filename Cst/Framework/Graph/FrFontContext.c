#include <Framework/Graph/FrFontContext.h>
#include <Framework/Graph/FrIDraw.h>
#include <Framework/Graph/FrDraw.h>

SYS_DEFINE_TYPE(FrFontContext, fr_font_context, FR_TYPE_CONTEXT);


void fr_font_context_draw_text(FrFontContext* self, PangoLayout* layout, SysInt x, SysInt y) {
  FrContext *cr = FR_CONTEXT(self);

  fr_context_move_to(cr, x, y);
  fr_context_show_layout(cr, layout);
}

void fr_font_context_set_color(FrFontContext* self, FrColor* color) {
  FrContext* ctx = FR_CONTEXT(self);

  fr_context_set_color(ctx, color);
}

void fr_font_context_show_text(FrFontContext* self, PangoLayout *layout,
  SysInt x, SysInt y, SysInt m1, SysInt m0) {
  FrIDrawInterface* iface = fr_draw_get_iface();
  FrContext* ctx = FR_CONTEXT(self);

  iface->move_to(ctx->cr, x + m1, y + m0);
  iface->show_layout(ctx->cr, x + m1, y + m0);
}

void fr_font_context_update_layout(FrFontContext* self, PangoLayout* layout) {
  FrContext* ctx = FR_CONTEXT(self);

  fr_i_draw_update_layout(ctx->cr, layout);
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

