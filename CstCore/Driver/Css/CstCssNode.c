#include <CstCore/Driver/Css/CstCssNode.h>

#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstLayout.h>

static SysHashTable* g_css_node_ht = NULL;

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

void cst_css_node_set_css_ptype(CstCssNode *self, SysInt css_ptype) {
  sys_return_if_fail(self != NULL);

  self->css_ptype = css_ptype;
}

SysInt cst_css_node_get_css_ptype(CstCssNode *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->css_ptype;
}

void cst_css_node_set_name(CstCssNode *self, const SysChar* name) {
  sys_return_if_fail(self != NULL);

  if(self->name) {

    sys_clear_pointer(&self->name, sys_free);
  }

  self->name = sys_strdup(name);
}

const SysChar* cst_css_node_get_name(CstCssNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

CstCssNode *cst_css_node_get_g_css_node(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, NULL);

  return sys_hash_table_lookup(g_css_node_ht, (const SysPointer)name);
}

void cst_css_node_set_g_css_node(CstCssNode *o) {
  sys_return_if_fail(o != NULL);

  sys_hash_table_insert(g_css_node_ht, (SysPointer)cst_css_node_get_name(o), (SysPointer)o);
}

void cst_css_node_setup(void) {
  sys_assert(g_css_node_ht == NULL);

  g_css_node_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
}

void cst_css_node_teardown(void) {

  sys_clear_pointer(&g_css_node_ht, sys_hash_table_unref);
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

