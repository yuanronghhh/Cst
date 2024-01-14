#include <CstCore/Front/Common/CstText.h>

#include <CstCore/Front/Common/CstLBoxContext.h>
#include <CstCore/Front/Common/CstTextContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstNode.h>


SYS_DEFINE_TYPE(CstText, cst_text, CST_TYPE_RENDER_NODE);


CstRenderNode* cst_text_new(void) {
  return sys_object_new(CST_TYPE_TEXT, NULL);
}

void cst_text_set_text(CstText* self, const SysChar *text) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(text != NULL);

  pango_layout_set_text(self->playout, text, -1);
}

const SysChar* cst_text_get_text(CstText* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return pango_layout_get_text(self->playout);
}

void cst_text_set_font_size(CstText *self, SysInt font_size) {
  sys_return_if_fail(self != NULL);

  pango_font_description_set_size(self->font_desc, font_size * PANGO_SCALE);
}

SysInt cst_text_get_font_size(CstText *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return pango_font_description_get_size(self->font_desc);
}

void cst_text_set_font_desc(CstText *self, const SysChar *desc) {
  sys_return_if_fail(self != NULL);

  if (self->font_desc) {
    sys_clear_pointer(&self->font_desc, pango_font_description_free);
  }

  self->font_desc = pango_font_description_from_string(desc);
}

void cst_text_set_alignment(CstText* self, SysInt align) {
  sys_return_if_fail(self != NULL);

  pango_layout_set_alignment(self->playout, align);
}

SysObject* cst_text_dclone_i(SysObject *o) {
  CstText *ntext;
  CstText *otext;

  SysObject *n = SYS_OBJECT_CLASS(cst_text_parent_class)->dclone(o);

  ntext = CST_TEXT(n);
  otext = CST_TEXT(o);

  ntext->playout = otext->playout ? pango_layout_copy(otext->playout) : NULL;
  ntext->font_desc = otext->font_desc ? pango_font_description_copy(otext->font_desc) : NULL;

  return n;
}

static void cst_text_construct_i(CstRenderNode *o, CstNode *node) {
  sys_return_if_fail(o != NULL);

  CST_RENDER_NODE_CLASS(cst_text_parent_class)->construct(o, node);
}

#if 0
static void cst_text_repaint_i(CstRenderNode *rnode, CstLayout *layout) {
  CstText *self = CST_TEXT(rnode);

  FRDrawLayout *playout = self->playout;
  FRDraw *draw = cst_layout_get_draw(layout);
  const FRRect *bound = cst_node_get_bound(rnode);
  const FRSInt4 *m4 = cst_node_get_margin(rnode);

  fr_draw_show_text(draw, playout, bound->x, bound->y, m4->m1, m4->m0);
}
#endif

void cst_text_get_size(CstRenderNode *o, SysInt *width, SysInt *height) {
  sys_return_if_fail(o != NULL);

  CstText *self = CST_TEXT(o);
  PangoLayout *playout = self->playout;

  fr_layout_get_pixel_size(playout, width, height);
}

/* object api */
static void cst_text_init(CstText *self) {
  CstRenderNode *rnode;
  CstRenderContext *rctx;
  PangoFontMap *font_map;
  PangoContext *pctx;

  rnode = CST_RENDER_NODE(self);
  rctx = cst_text_context_new_I();
  font_map = pango_cairo_font_map_get_default();
  pctx = pango_font_map_create_context(font_map);
  self->playout = pango_layout_new (pctx);

  cst_render_node_set_rctx(rnode, rctx);

  g_object_unref(pctx);
}

static void cst_text_dispose(SysObject* o) {
  CstText *self = CST_TEXT(o);

  if(self->font_desc) {
    pango_font_description_free(self->font_desc);
  }

  if (self->playout) {
    sys_clear_pointer(&self->playout, g_object_unref);
  }

  SYS_OBJECT_CLASS(cst_text_parent_class)->dispose(o);
}

static void cst_text_class_init(CstTextClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstRenderNodeClass *ncls = CST_RENDER_NODE_CLASS(cls);

  ocls->dispose = cst_text_dispose;
  ocls->dclone = cst_text_dclone_i;

  ncls->construct = cst_text_construct_i;
}
