#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCssNode.h>


SYS_DEFINE_TYPE(CstCssPair, cst_css_pair, SYS_TYPE_OBJECT);


SysObject *cst_css_pair_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(cst_css_pair_parent_class)->dclone(o);

  CstCssPair *nself = CST_CSS_PAIR(n);
  CstCssPair *oself = CST_CSS_PAIR(o);

  nself->css_node = oself->css_node;
  nself->value = (CstCssValue *)sys_object_dclone(oself->value);

  sys_object_ref(oself->css_node);

  return n;
}

void cst_css_pair_calc(CstCssPair *self, CstRenderNode *rnode, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  CstCssNode *node = self->css_node;

  if(cst_css_node_check(node, layout)) {

    cst_css_node_set_value(node, rnode, layout, self);
  }
}

void cst_css_pair_set_x(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_CLOSURE)) {
    CstCssClosure *v = cst_css_value_get_v_closure(self->value);

    cst_css_closure_calc(v, layout, rnode);
  }

  if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_INT)) {
    SysInt v = cst_css_value_get_v_int(self->value);

    cst_layout_node_set_x(lnode, v);
  }
}

void cst_css_pair_set_y(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  SysInt v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  cst_layout_node_set_y(lnode, v);
}

void cst_css_pair_set_width(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_CLOSURE)) {
    CstCssClosure *v = cst_css_value_get_v_closure(self->value);

    cst_css_closure_calc(v, layout, rnode);
  } else if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_INT)) {
    SysInt v = cst_css_value_get_v_int(self->value);
    cst_layout_node_set_width(lnode, v);

  } else {

    sys_warning_N("css width is not valid int or closure: %s", cst_css_node_name(self->css_node));
  }
}

void cst_css_pair_set_height(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  SysInt v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  cst_layout_node_set_height(lnode, v);
}

void cst_css_pair_set_layer(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);

  CstLayer *tolayer;
  CstRender *v_render;
  SysInt v;

  v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  v_render = cst_layout_get_render(layout);
  tolayer = cst_render_get_layer_by_type(v_render, v);

  cst_render_node_change_to_layer(rnode, tolayer);
}

void cst_css_pair_set_margin(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  FRSInt4 *v = cst_css_value_get_v_m4(self->value);
  sys_return_if_fail(v != NULL);

  cst_layout_node_set_margin(lnode, v);
}

void cst_css_pair_set_border(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  FRSInt4 *v = cst_css_value_get_v_m4(self->value);
  sys_return_if_fail(v != NULL);

  cst_layout_node_set_border(lnode, v);
}

void cst_css_pair_set_padding(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  FRSInt4 *v = cst_css_value_get_v_m4(self->value);
  sys_return_if_fail(v != NULL);

  cst_layout_node_set_padding(lnode, v);
}

void cst_css_pair_set_font_family(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  const SysChar *v = cst_css_value_get_v_string(self->value);
  sys_return_if_fail(v != NULL);

  cst_text_set_font_desc(CST_TEXT(rnode), v);
}

void cst_css_pair_set_font_size(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstLayoutNode *lnode = CST_LAYOUT_NODE(rnode);

  SysInt v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  cst_text_set_font_size(CST_TEXT(rnode), v);
}

void cst_css_pair_set_wrap(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);

  SysBool v = cst_css_value_get_v_bool(self->value);

  cst_render_node_set_wrap(rnode, v);
}

void cst_css_pair_set_color(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);

  FRColor* v = cst_css_value_get_v_color(self->value);
  sys_return_if_fail(v != NULL);

  FRDraw *draw = cst_layout_get_draw(layout);
  sys_return_if_fail(draw != NULL);

  fr_draw_set_color(draw, v);
}

void cst_css_pair_width_percent(CstRenderNode *rnode, CstLayout *layout, SysInt64 d) {
  sys_return_if_fail(rnode != NULL);

  const FRRect *bound;
  SysInt pwidth;
  FRSInt4 m4;
  CstLayoutNode* lnode;

  lnode = CST_LAYOUT_NODE(rnode);
  bound = cst_layout_node_get_bound(lnode);

  cst_layout_node_get_mbp(lnode, &m4);
  pwidth = bound->width - m4.m1 - m4.m3;

  cst_layout_node_set_width(lnode, (SysInt)(pwidth * d * 0.01));
}

void cst_css_pair_height_percent(CstRenderNode * rnode, CstLayout *layout, SysInt64 d) {
  sys_return_if_fail(rnode != NULL);

  const FRRect* bound;
  SysInt pheight;
  FRSInt4 m4;
  CstLayoutNode* lnode;

  lnode = CST_LAYOUT_NODE(rnode);
  bound = cst_layout_node_get_bound(lnode);

  cst_layout_node_get_mbp(lnode, &m4);
  pheight = bound->height - m4.m1 - m4.m3;

  cst_layout_node_set_height(lnode, (SysInt)(pheight * d * 0.01));
}

void cst_css_pair_setup(void) {
  CstCssNode *node;

  cst_css_node_setup();

  node = cst_css_node_new_I("x"            ,  CST_CSS_PROP_X            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_x);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("x"            ,  CST_CSS_PROP_X            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_x);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("y"            ,  CST_CSS_PROP_Y            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_y);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("width"        ,  CST_CSS_PROP_W            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_width);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("height"       ,  CST_CSS_PROP_H            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_height);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("layer"        ,  CST_CSS_PROP_LAYER        ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_layer);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("margin"       ,  CST_CSS_PROP_MARGIN       ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_margin);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("border"       ,  CST_CSS_PROP_BORDER       ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_border);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("padding"      ,  CST_CSS_PROP_PADDING      ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_padding);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("font-family"  ,  CST_CSS_PROP_FONT_FAMILY  ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_font_family);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("font-size"    ,  CST_CSS_PROP_FONT_SIZE    ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_font_size);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("wrap"         ,  CST_CSS_PROP_WRAP         ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_wrap);
  cst_css_node_set_g_css_node(node);
  node = cst_css_node_new_I("color"        ,  CST_CSS_PROP_COLOR        ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_color);
  cst_css_node_set_g_css_node(node);
}

void cst_css_pair_teardown(void) {
  cst_css_node_teardown();
}

/* object api */
static void cst_css_pair_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstCssPair *self = CST_CSS_PAIR(o);

  sys_clear_pointer(&self->value, _sys_object_unref);
  sys_clear_pointer(&self->css_node, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_css_pair_parent_class)->dispose(o);
}

static void cst_css_pair_construct(CstCssPair* self, CstCssNode *node, CstCssValue *value) {
  self->css_node = node;
  self->value = value;

  sys_object_ref(node);
}

CstCssPair* cst_css_pair_new_by_key(const SysChar *key) {
  CstCssValue* o;
  CstCssPair* pair;

  CstCssNode* node = cst_css_node_get_g_css_node(key);
  if (node == NULL) {

    sys_warning_N("Not found css by name: %s", key);
    return NULL;
  }

  o = cst_css_value_new();
  pair = cst_css_pair_new_I(node, o);

  return pair;
}

CstCssPair *cst_css_pair_new_I(CstCssNode* node, CstCssValue* value) {
  sys_return_val_if_fail(node != NULL, NULL);
  sys_return_val_if_fail(value != NULL, NULL);

  CstCssPair *o = cst_css_pair_new();

  cst_css_pair_construct(o, node, value);

  return o;
}

CstCssPair *cst_css_pair_new(void) {
  return sys_object_new(CST_TYPE_CSS_PAIR, NULL);
}

static void cst_css_pair_class_init(CstCssPairClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_pair_dispose;
  ocls->dclone = cst_css_pair_dclone_i;
}

static void cst_css_pair_init(CstCssPair *self) {
}

