#include <CstCore/Driver/Css/CstCssNode.h>
#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>


static SysHashTable* gcss_node_ht = NULL;


SYS_DEFINE_TYPE(CstCssNode, cst_css_node, SYS_TYPE_OBJECT);


/* css node */
const SysChar* cst_css_node_name(CstCssNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

SysBool cst_css_node_check(CstCssNode *self, CstLayout *layout) {
  sys_return_val_if_fail(self != NULL, false);
  SysInt state = cst_layout_get_state(layout);

  if(!(self->css_state & state)) {
    return false;
  }

  return true;
}

void cst_css_node_set_value(CstCssNode *self, CstRenderNode *rnode, CstLayout *layout, SysPointer user_data) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(rnode != NULL);

  sys_return_if_fail(self->set_value != NULL);

  self->set_value(rnode, layout, user_data);
}

CstCssNode *cst_css_node_lookup(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, NULL);
  sys_return_val_if_fail(gcss_node_ht != NULL, NULL);

  return sys_hash_table_lookup(gcss_node_ht, (const SysPointer)name);
}

void cst_css_node_bind_map(SysChar* name, SysInt css_type, SysInt css_state, CstCssFunc set_value) {
  CstCssNode* node = cst_css_node_new_I(name, css_type, css_state, set_value);

  sys_hash_table_insert(gcss_node_ht, (SysPointer)node->name, (SysPointer)node);
}

void cst_css_node_set_css_ptype(CstCssNode *self, SysInt css_ptype) {
  sys_return_if_fail(self != NULL);

  self->css_ptype = css_ptype;
}

SysInt cst_css_node_get_css_ptype(CstCssNode *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->css_ptype;
}

void cst_css_node_setup(void) {
  sys_assert(gcss_node_ht == NULL && "css node should setup only once.");

  gcss_node_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
}

void cst_css_node_teardown(void) {
  sys_assert(gcss_node_ht != NULL && "css value should init before use.");

  sys_hash_table_unref(gcss_node_ht);
}

/* object api */
static void cst_css_node_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_css_node_parent_class)->dispose(o);
}

static void cst_css_node_construct(CstCssNode* self, SysChar* name, SysInt css_ptype, SysInt css_state, CstCssFunc set_value) {
  self->name = name;
  self->css_ptype = css_ptype;
  self->css_state = css_state;
  self->set_value = set_value;
}

CstCssNode* cst_css_node_new_I(SysChar* name, SysInt css_ptype, SysInt css_state, CstCssFunc set_value) {
  sys_return_val_if_fail(name != NULL, NULL);
  sys_return_val_if_fail(set_value != NULL, NULL);

  CstCssNode *o = cst_css_node_new();

  cst_css_node_construct(o, name, css_ptype, css_state, set_value);

  return o;
}

CstCssNode *cst_css_node_new(void) {
  return sys_object_new(CST_TYPE_CSS_NODE, NULL);
}

static void cst_css_node_class_init(CstCssNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_node_dispose;
}

static void cst_css_node_init(CstCssNode *self) {
}

