#include <CstCore/Front/Common/CstTextContext.h>

#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstTextContext, cst_text_context, CST_TYPE_RENDER_CONTEXT);


static void cst_text_relayout_i(CstRenderContext *ctx, CstRenderNode *o, CstLayout *layout) {
  CstText *text = CST_TEXT(o);

  FRDraw *draw;
  SysInt width = 0;
  SysInt height = 0;

  FRDrawLayout *playout = text->playout;
  PangoFontDescription *font_desc = text->font_desc;

  draw = cst_layout_get_draw(layout);

  fr_layout_set_font_description (playout, font_desc);
  fr_layout_get_pixel_size(playout, &width, &height);
  fr_draw_layout_layout(draw, playout);
  cst_render_node_set_size(o, width, height);
}

/* sys object api */
CstRenderContext* cst_text_context_new(void) {
  return sys_object_new(CST_TYPE_TEXT_CONTEXT, NULL);
}

CstRenderContext* cst_text_context_new_I(void) {
  CstRenderContext* o = cst_text_context_new();

  return o;
}

static void cst_text_context_init(CstTextContext *o) {
}

static void cst_text_context_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_text_context_parent_class)->dispose(o);
}

static void cst_text_context_class_init(CstTextContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderContextClass* rcls = CST_RENDER_CONTEXT_CLASS(cls);

  ocls->dispose = cst_text_context_dispose;

  rcls->layout_self = cst_text_relayout_i;
}
