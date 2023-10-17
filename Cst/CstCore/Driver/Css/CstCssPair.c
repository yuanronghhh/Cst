#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>


struct _CstCssNode {
  SysChar* name;
  SysInt css_ptype;
  SysInt css_state;

  CstCssFunc set_value;
};

static const SysChar* CST_CSS_NODE_NAMES[] = {
  "x", "y", "w", "h", "position",
  "margin", "border", "padding", "gap",
  "font_size", "font_family", "font",
  "wrap",
  "color",
  "bacground",
  "border-radius",
  "invalid"
};
static SysHashTable* gcss_node_ht = NULL;

SYS_DEFINE_TYPE(CstCssPair, cst_css_pair, SYS_TYPE_OBJECT);

/* css node */
CstCssNode* cst_css_node_new(SysChar* name, SysInt css_ptype, SysInt css_state, CstCssFunc set_value) {
  CstCssNode* self = sys_new0_N(CstCssNode, 1);

  self->name = name;
  self->css_ptype = css_ptype;
  self->css_state = css_state;
  self->set_value = set_value;

  return self;
}

void cst_css_node_free(CstCssNode* self) {
  sys_free_N(self);
}

const SysChar* cst_css_node_name(CstCssNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

SysBool cst_css_node_check(CstCssNode *self, SysInt state) {
  sys_return_val_if_fail(self != NULL, false);

  return self->css_state & state;
}

CstCssNode *cst_css_node_lookup(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, NULL);
  sys_return_val_if_fail(gcss_node_ht != NULL, NULL);

  return sys_hash_table_lookup(gcss_node_ht, (const SysPointer)name);
}

CST_CSS_PROP_ENUM cst_css_node_get_css_ptype(const SysChar *name) {
  CstCssNode *node = sys_hash_table_lookup(gcss_node_ht, (const SysPointer)name);
  if (node == NULL) {

    return -1;
  }

  return node->css_ptype;
}

CstCssPair *cst_css_pair_dclone(CstCssPair *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  CstCssValue *nvalue = cst_css_value_dclone(o->value);
  CstCssPair *npair = cst_css_pair_new_I(o->css_node, nvalue);

  return npair;
}

void cst_css_pair_calc(CstCssPair *self, CstRenderNode *rnode, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
  CstCssNode *node = self->css_node;

  if (cst_layout_is_state(layout, node->css_state)) {
    node->set_value(rnode, layout, self);
  }
}

void cst_css_pair_set_x(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);

  CstNode *node = cst_render_node_get_node(rnode);

  if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_CLOSURE)) {
    CstCssClosure *v = cst_css_value_get_v_closure(self->value);

    cst_css_closure_calc(v, layout, rnode);
  }

  if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_INT)) {
    SysInt v = cst_css_value_get_v_int(self->value);

    cst_layout_node_set_x(CST_LAYOUT_NODE(node), v);
  }

}

void cst_css_pair_set_y(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode *node = cst_render_node_get_node(rnode);

  SysInt v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  cst_layout_node_set_y(CST_LAYOUT_NODE(node), v);
}

void cst_css_pair_set_width(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode* node = cst_render_node_get_node(rnode);

  if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_CLOSURE)) {
    CstCssClosure *v = cst_css_value_get_v_closure(self->value);

    cst_css_closure_calc(v, layout, rnode);
  }

  if (cst_css_value_is_d_type(self->value, CST_CSS_VALUE_INT)) {
    SysInt v = cst_css_value_get_v_int(self->value);
    cst_layout_node_set_width(CST_LAYOUT_NODE(node), v);
  }
}

void cst_css_pair_set_height(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode *node = cst_render_node_get_node(rnode);

  SysInt v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  cst_layout_node_set_height(CST_LAYOUT_NODE(node), v);
}

void cst_css_pair_set_position(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode *node = cst_render_node_get_node(rnode);
  CstRenderContext* rctx = cst_render_node_get_render_ctx(rnode);

  SysInt v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  cst_render_context_set_position(rctx, v);
}

void cst_css_pair_set_margin(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode* node = cst_render_node_get_node(rnode);

  FRSInt4 *v = cst_css_value_get_v_m4(self->value);
  sys_return_if_fail(v != NULL);

  cst_layout_node_set_margin(CST_LAYOUT_NODE(node), v);
}

void cst_css_pair_set_border(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode* node = cst_render_node_get_node(rnode);

  FRSInt4 *v = cst_css_value_get_v_m4(self->value);
  sys_return_if_fail(v != NULL);

  cst_layout_node_set_border(CST_LAYOUT_NODE(node), v);
}

void cst_css_pair_set_padding(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode* node = cst_render_node_get_node(rnode);

  FRSInt4 *v = cst_css_value_get_v_m4(self->value);
  sys_return_if_fail(v != NULL);

  cst_layout_node_set_padding(CST_LAYOUT_NODE(node), v);
}

void cst_css_pair_set_font_family(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode* node = cst_render_node_get_node(rnode);

  const SysChar *v = cst_css_value_get_v_string(self->value);
  sys_return_if_fail(v != NULL);

  cst_text_set_font_desc(CST_TEXT(node), v);
}

void cst_css_pair_set_font_size(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode* node = cst_render_node_get_node(rnode);

  SysInt v = cst_css_value_get_v_int(self->value);
  sys_return_if_fail(v != -1);

  cst_text_set_font_size(CST_TEXT(node), v);
}

void cst_css_pair_set_wrap(CstRenderNode* rnode, CstLayout *layout, SysPointer user_data) {
  CstCssPair *self = user_data;
  sys_return_if_fail(self != NULL);
  CstNode* node = cst_render_node_get_node(rnode);
  CstRenderContext* rctx = cst_render_node_get_render_ctx(rnode);

  SysBool v = cst_css_value_get_v_bool(self->value);

  cst_render_context_set_wrap(rctx, v);
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
  CstRenderNode *pnode;
  CstLayoutNode* lnode;
  CstNode* node;

  node = cst_render_node_get_node(rnode);
  lnode = CST_LAYOUT_NODE(node);
  pnode = cst_render_node_get_parent(rnode);
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
  CstRenderNode* pnode;
  CstLayoutNode* lnode;
  CstNode* node;

  node = cst_render_node_get_node(rnode);
  lnode = CST_LAYOUT_NODE(node);
  pnode = cst_render_node_get_parent(rnode);
  bound = cst_layout_node_get_bound(lnode);

  cst_layout_node_get_mbp(lnode, &m4);
  pheight = bound->height - m4.m1 - m4.m3;

  cst_layout_node_set_height(lnode, (SysInt)(pheight * d * 0.01));
}

static void cst_css_pair_bind_map(SysChar* name, SysInt css_type, SysInt css_state, CstCssFunc set_value) {
  CstCssNode* node = cst_css_node_new(name, css_type, css_state, set_value);

  sys_hash_table_insert(gcss_node_ht, (SysPointer)node->name, (SysPointer)node);
}

void cst_css_pair_setup(void) {
  sys_assert(gcss_node_ht == NULL && "css value should setup only once.");

  gcss_node_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, cst_css_node_free);

  cst_css_pair_bind_map("x"            ,  CST_CSS_PROP_X            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_x);
  cst_css_pair_bind_map("y"            ,  CST_CSS_PROP_Y            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_y);
  cst_css_pair_bind_map("width"        ,  CST_CSS_PROP_W            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_width);
  cst_css_pair_bind_map("height"       ,  CST_CSS_PROP_H            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_height);
  cst_css_pair_bind_map("position"     ,  CST_CSS_PROP_POSITION     ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_position);
  cst_css_pair_bind_map("margin"       ,  CST_CSS_PROP_MARGIN       ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_margin);
  cst_css_pair_bind_map("border"       ,  CST_CSS_PROP_BORDER       ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_border);
  cst_css_pair_bind_map("padding"      ,  CST_CSS_PROP_PADDING      ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  , cst_css_pair_set_padding);
  cst_css_pair_bind_map("font-family"  ,  CST_CSS_PROP_FONT_FAMILY  ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_font_family);
  cst_css_pair_bind_map("font-size"    ,  CST_CSS_PROP_FONT_SIZE    ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_font_size);
  cst_css_pair_bind_map("wrap"         ,  CST_CSS_PROP_WRAP         ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_wrap);
  cst_css_pair_bind_map("color"        ,  CST_CSS_PROP_COLOR        ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   , cst_css_pair_set_color);
}

void cst_css_pair_teardown(void) {
  sys_assert(gcss_node_ht != NULL && "css value should init before use.");

  sys_hash_table_unref(gcss_node_ht);
}

/* object api */
static void cst_css_pair_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstCssPair *self = CST_CSS_PAIR(o);

  sys_clear_pointer(&self->value, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_css_pair_parent_class)->dispose(o);
}

static void cst_css_pair_construct(CstCssPair* self, CstCssNode *node, CstCssValue *value) {
  self->css_node = node;
  self->value = value;
}

CstCssPair* cst_css_pair_new_by_key(const SysChar *key) {
  CstCssValue* o;
  CstCssPair* pair;

  CstCssNode* node = cst_css_node_lookup(key);
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
}

static void cst_css_pair_init(CstCssPair *self) {
}

