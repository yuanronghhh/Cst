#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Driver/Css/CstCssValue.h>


struct _CstCssNode {
  SysChar* name;
  SysInt css_ptype;
  SysInt css_state;
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
CstCssNode* cst_css_node_new(SysChar* name, SysInt css_ptype, SysInt css_state) {
  CstCssNode* self = sys_new0_N(CstCssNode, 1);

  self->name = name;
  self->css_ptype = css_ptype;
  self->css_state = css_state;

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

CstCssPair *cst_css_pair_clone(CstCssPair *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  CstCssValue *nvalue = cst_css_value_dclone(o->value);
  CstCssPair *npair = cst_css_pair_new_I(o->css_node, nvalue);

  return npair;
}

void cst_css_pair_calc(CstCssPair *pair, CstRenderNode *render_node, CstLayout *layout) {

  cst_css_value_set_value(pair->value, render_node, layout);
}

static void cst_css_pair_bind_map(SysChar* name, SysInt css_type, SysInt css_state) {
  CstCssNode* node = cst_css_node_new(name, css_type, css_state);

  sys_hash_table_insert(gcss_node_ht, (SysPointer)node->name, (SysPointer)node);
}

void cst_css_pair_setup(void) {
  sys_assert(gcss_node_ht == NULL && "css value should setup only once.");

  gcss_node_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, cst_css_node_free);

  cst_css_pair_bind_map("x"            ,  CST_CSS_PROP_X            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("y"            ,  CST_CSS_PROP_Y            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("width"        ,  CST_CSS_PROP_W            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("height"       ,  CST_CSS_PROP_H            ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("position"     ,  CST_CSS_PROP_POSITION     ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("margin"       ,  CST_CSS_PROP_MARGIN       ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("border"       ,  CST_CSS_PROP_BORDER       ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("padding"      ,  CST_CSS_PROP_PADDING      ,  CST_RENDER_STATE_LAYOUT  |  CST_RENDER_STATE_RELAYOUT  );
  cst_css_pair_bind_map("font-family"  ,  CST_CSS_PROP_FONT_FAMILY  ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   );
  cst_css_pair_bind_map("font-size"    ,  CST_CSS_PROP_FONT_SIZE    ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   );
  cst_css_pair_bind_map("wrap"         ,  CST_CSS_PROP_WRAP         ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   );
  cst_css_pair_bind_map("color"        ,  CST_CSS_PROP_COLOR        ,  CST_RENDER_STATE_PAINT   |  CST_RENDER_STATE_REPAINT   );
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

