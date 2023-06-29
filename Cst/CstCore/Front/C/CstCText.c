#include <CstCore/Front/Common/CstTextPrivate.h>
#include <CstCore/Front/C/CstCText.h>
#include <CstCore/Driver/CstRender.h>


typedef enum _CST_TEXT_DIRTY_ENUM {
    CST_TEXT_DIRTY = CST_DIRTY_DIRTY,
    CST_TEXT_DIRTY_FONT_SIZE = 1 << 3,
    CST_TEXT_DIRTY_OVERLAY = 1 << 4,
} CST_TEXT_DIRTY_ENUM;


struct _CstTextPrivate {
  PangoLayout *layout;
  PangoFontDescription *font_desc;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstText, cst_text, CST_TYPE_WIDGET);

CstNode* cst_text_new(void) {
  return sys_object_new(CST_TYPE_TEXT, NULL);
}

void cst_text_set_text(CstText* self, const SysChar *text) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(text != NULL);

  CstTextPrivate *priv = self->priv;

  sys_assert(priv->layout != NULL);

  pango_layout_set_text(priv->layout, text, -1);
}

const SysChar* cst_text_get_text(CstText* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstTextPrivate* priv = self->priv;

  return pango_layout_get_text(priv->layout);
}

void cst_text_set_font_size(CstText *self, SysInt font_size) {
  sys_return_if_fail(self != NULL);

  CstTextPrivate *priv = self->priv;

  pango_font_description_set_size(priv->font_desc, font_size * PANGO_SCALE);
}

SysInt cst_text_get_font_size(CstText *self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstTextPrivate *priv = self->priv;

  return pango_font_description_get_size(priv->font_desc);
}

void cst_text_set_font_desc(CstText *self, const SysChar *desc) {
  sys_return_if_fail(self != NULL);

  CstTextPrivate *priv = self->priv;

  if (priv->font_desc) {
    sys_clear_pointer(&priv->font_desc, pango_font_description_free);
  }

  priv->font_desc = pango_font_description_from_string(desc);
}

void cst_text_set_alignment(CstText* self, SysInt align) {
  sys_return_if_fail(self != NULL);

  CstTextPrivate* priv = self->priv;

  pango_layout_set_alignment(priv->layout, align);
}

CstNode* cst_text_dclone_i(CstNode *node) {
  CstText *ntext;
  CstText *otext;

  CstNode *nnode;

  nnode = CST_NODE_CLASS(cst_text_parent_class)->dclone(node);
  ntext = CST_TEXT(nnode);
  otext = CST_TEXT(node);

  CstTextPrivate *opriv = otext->priv;
  CstTextPrivate *npriv = ntext->priv;

  npriv->layout = opriv->layout ? pango_layout_copy(opriv->layout) : NULL;
  npriv->font_desc = opriv->font_desc ? pango_font_description_copy(opriv->font_desc) : NULL;

  return nnode;
}

static void cst_text_construct_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
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

CstNode *cst_text_realize_i (CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  CstNode *nnode = CST_NODE_CLASS(cst_text_parent_class)->realize(v_module, ncomp_node, v_parent, v_node, v_render);

  return nnode;
}

static void cst_text_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  CstText *self = CST_TEXT(v_node);
  CstTextPrivate *priv = self->priv;
  SysInt m0, m1, m2, m3;

  const FRRect *bound = cst_node_get_bound(v_node);
  PangoLayout *layout = priv->layout;

  cst_node_get_mbp(v_node, &m0, &m1, &m2, &m3);

  if(cst_node_is_dirty(v_node)) {

    fr_context_move_to(cr, bound->x + m1, bound->y + m0);
    pango_cairo_show_layout (cr, layout);

    cst_node_set_need_repaint(v_node, false);
  }


  CST_NODE_CLASS(cst_text_parent_class)->repaint(v_module, v_parent, v_node, cr, draw, state);
}

static void cst_text_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state) {
  CstText *self = CST_TEXT(v_node);
  CstTextPrivate* priv = self->priv;

  SysInt width = 0;
  SysInt height = 0;

  PangoLayout *layout = priv->layout = pango_cairo_create_layout (cr);
  PangoFontDescription *font_desc = priv->font_desc;

  pango_layout_set_font_description (layout, font_desc);

  if (cst_node_is_dirty(v_node)) {
    pango_cairo_update_layout (cr, layout);
    pango_layout_get_pixel_size (layout, &width, &height);

    cst_node_set_size(v_node, width, height);
    cst_node_set_need_relayout(v_node, false);
  }

  cst_node_relayout_h(v_module, v_parent, v_node, cr, draw, state);

  CST_NODE_CLASS(cst_text_parent_class)->relayout(v_module, v_parent, v_node, cr, draw, state);
}

/* object api */
static void cst_text_init(CstText *self) {
  self->priv = cst_text_get_private(self);
  CstNode *node = CST_NODE(self);

  cst_node_set_name(node, "Text");
}

static void cst_text_dispose(SysObject* o) {
  CstText *self = CST_TEXT(o);
  CstTextPrivate* priv = self->priv;

  if(priv->font_desc) {
    pango_font_description_free(priv->font_desc);
  }

  if (priv->layout) {
    sys_clear_pointer(&priv->layout, g_object_unref);
  }

  SYS_OBJECT_CLASS(cst_text_parent_class)->dispose(o);
}

static void cst_text_class_init(CstTextClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);
  CstNodeClass *ncls = CST_NODE_CLASS(cls);

  ocls->dispose = cst_text_dispose;

  ncls->dclone = cst_text_dclone_i;
  ncls->construct = cst_text_construct_i;
  ncls->relayout = cst_text_relayout_i;
  ncls->repaint = cst_text_repaint_i;
  ncls->realize = cst_text_realize_i;
}
