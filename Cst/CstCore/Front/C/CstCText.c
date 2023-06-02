#include <CstCore/Front/Common/CstTextPrivate.h>
#include <CstCore/Front/C/CstCText.h>
#include <CstCore/Driver/CstRender.h>


struct _CstTextPrivate {
  SysChar *text;

  PangoLayout *layout;
  PangoFontDescription *font_desc;
  SysInt font_size;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstText, cst_text, CST_TYPE_WIDGET);

CstNode* cst_text_new(void) {
  return sys_object_new(CST_TYPE_TEXT, NULL);
}

void cst_text_set_text(CstText* self, const SysChar *text) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(text != NULL);

  CstTextPrivate *priv = self->priv;

  if(priv->text) {
    sys_clear_pointer(&priv->text, sys_free);
  }

  priv->text = sys_strdup(text);
}

void cst_text_set_font_size(CstText *self, SysInt font_size) {
  sys_return_if_fail(self != NULL);

  CstTextPrivate *priv = self->priv;

  priv->font_size = font_size;
}

SysInt cst_text_get_font_size(CstText *self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstTextPrivate *priv = self->priv;

  return priv->font_size;
}

void cst_text_set_font_desc(CstText *self, const SysChar *desc) {
  sys_return_if_fail(self != NULL);

  CstTextPrivate *priv = self->priv;

  if (priv->font_desc) {
    sys_clear_pointer(&priv->font_desc, pango_font_description_free);
  }

  priv->font_desc = pango_font_description_from_string(desc);
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
  npriv->text = opriv->text ? sys_strdup(opriv->text) : NULL;
  npriv->font_size = opriv->font_size;

  return nnode;
}

static void cst_text_construct_i(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props) {
  sys_return_if_fail(v_module != NULL);
  sys_return_if_fail(v_props != NULL);

  CstText* self = CST_TEXT(v_node);

  const SysChar *value;

  CST_NODE_CLASS(cst_text_parent_class)->construct(v_module, v_component, v_parent, v_node, v_props);

  value = v_props->v_value;
  if (value) {
    cst_text_set_text(self, value);
  }
}

CstNode *cst_text_realize_i (CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render) {
  CstNode *nnode = CST_NODE_CLASS(cst_text_parent_class)->realize(v_module, ncomp_node, v_parent, v_node, v_render);

  CstText *self = CST_TEXT(nnode);

  CstTextPrivate* priv = self->priv;
  FRContext *cr = cst_render_get_cr(v_render);

  sys_assert(priv->layout == NULL && "layout should be null before realize");

  sys_debug_N("%d", pthread_self());
  priv->layout = pango_cairo_create_layout (cr);

  return nnode;
}

static void cst_text_repaint_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  CstText *self = CST_TEXT(v_node);
  CstTextPrivate *priv = self->priv;
  SysInt m0, m1, m2, m3;

  const FRRect *bound = cst_node_get_bound(v_node);
  PangoLayout *layout = priv->layout;

  cst_node_get_mbp(v_node, &m0, &m1, &m2, &m3);
  fr_context_move_to(cr, bound->x + m1, bound->y + m0);
  pango_cairo_show_layout (cr, layout);
  sys_clear_pointer(&priv->layout, g_object_unref);

  CST_NODE_CLASS(cst_text_parent_class)->repaint(v_module, v_parent, v_node, cr);
}

static void cst_text_relayout_i(CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  CstText *self = CST_TEXT(v_node);
  CstTextPrivate* priv = self->priv;

  SysInt width = 0;
  SysInt height = 0;

  PangoLayout *layout = priv->layout;
  PangoFontDescription *font_desc = priv->font_desc;

  pango_layout_set_text (layout, priv->text, -1);
  pango_font_description_set_size (font_desc, priv->font_size * PANGO_SCALE);
  pango_layout_set_font_description (layout, font_desc);

  pango_cairo_update_layout (cr, layout);
  pango_layout_get_pixel_size (layout, &width, &height);
  cst_node_set_size(v_node, width, height);

  cst_node_relayout_h(v_module, v_parent, v_node, cr);

  CST_NODE_CLASS(cst_text_parent_class)->relayout(v_module, v_parent, v_node, cr);
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
  sys_free_N(priv->text);

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
