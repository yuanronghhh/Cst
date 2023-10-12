#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Driver/CstRender.h>


typedef enum _CST_TEXT_DIRTY_ENUM {
  CST_TEXT_DIRTY = CST_DIRTY_DIRTY,
  CST_TEXT_DIRTY_FONT_SIZE = 1 << 3,
  CST_TEXT_DIRTY_OVERLAY = 1 << 4,
} CST_TEXT_DIRTY_ENUM;


SYS_DEFINE_TYPE(CstText, cst_text, CST_TYPE_NODE);

CstNode* cst_text_new(void) {
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

CstNode* cst_text_dclone_i(CstNode *node) {
  CstText *ntext;
  CstText *otext;

  CstNode *nnode;

  nnode = CST_NODE_CLASS(cst_text_parent_class)->dclone(node);
  ntext = CST_TEXT(nnode);
  otext = CST_TEXT(node);

  ntext->playout = otext->playout ? pango_layout_copy(otext->playout) : NULL;
  ntext->font_desc = otext->font_desc ? pango_font_description_copy(otext->font_desc) : NULL;

  return nnode;
}

static void cst_text_construct_i(CstNodeProvider *provider, CstNodeProps *v_props) {
  sys_return_if_fail(v_module != NULL);
  sys_return_if_fail(v_props != NULL);

  const SysChar *value;
  CstText* self = CST_TEXT(v_node);

  CST_NODE_CLASS(cst_text_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);

  value = v_props->v_value;
  if (value) {

    cst_text_set_text(self, value);
  }
}

#if 0
static void cst_text_repaint_i(CstNode *node, CstLayout *layout) {
  CstText *self = CST_TEXT(v_node);
  CstTextPrivate *priv = self->priv;
  FRSInt4 m4;

  const FRRect *bound = cst_render_node_get_bound(v_node);
  PangoLayout *playout = self->playout;

  cst_render_node_get_margin(v_node, &m4);

  if(cst_render_node_is_dirty(v_node)) {

    fr_playout_show_text(draw, playout, bound, &m4);
    cst_render_node_set_need_repaint(v_node, false);
  }

  CST_NODE_CLASS(cst_text_parent_class)->repaint(self, layout);
}

static void cst_text_relayout_i(CstNode *self, CstLayout *layout) {
  CstText *self = CST_TEXT(v_node);
  SysInt width = 0;
  SysInt height = 0;

  PangoLayout *playout = self->playout;
  PangoFontDescription *font_desc = self->font_desc;
  FRContext *cr = fr_draw_get_cr(draw);

  pango_layout_set_font_description (playout, font_desc);

  if (cst_render_node_is_dirty(v_node)) {
    pango_cairo_update_layout (cr, playout);
    pango_layout_get_pixel_size (playout, &width, &height);

    cst_render_node_set_size(v_node, width, height);
    cst_render_node_set_need_relayout(v_node, false);
  }

  cst_render_node_relayout_h(self, layout);

  CST_NODE_CLASS(cst_text_parent_class)->relayout(self, layout);
}
#endif

/* object api */
static void cst_text_init(CstText *self) {
  CstNode *node = CST_NODE(self);

  PangoFontMap *font_map = pango_cairo_font_map_get_default();
  PangoContext *pctx = pango_font_map_create_context(font_map);

  self->playout = pango_layout_new (pctx);

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
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_text_dispose;

  ncls->dclone = cst_text_dclone_i;
  ncls->construct = cst_text_construct_i;
}
