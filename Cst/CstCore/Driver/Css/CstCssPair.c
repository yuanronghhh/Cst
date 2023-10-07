#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Driver/Css/CstCssValue.h>


SYS_DEFINE_TYPE(CstCssPair, cst_css_pair, SYS_TYPE_OBJECT);


CstCssPair *cst_css_pair_clone(CstCssPair *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  CstCssValue *nvalue = cst_css_value_dclone(o->value);
  CstCssPair *npair = cst_css_pair_new_I(o->key, nvalue);

  return npair;
}

void cst_css_pair_calc(CstCssPair *pair, CstRenderNode *render_node, CstLayout *layout) {
  cst_css_value_calc(pair->value, render_node, layout);
  cst_css_value_set_value(pair->value, render_node, layout);
}

/* object api */
static void cst_css_pair_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstCssPair *self = CST_CSS_PAIR(o);

  sys_clear_pointer(&self->key, sys_free);
  sys_clear_pointer(&self->value, _sys_object_unref);

  SYS_OBJECT_CLASS(cst_css_pair_parent_class)->dispose(o);
}

static void cst_css_pair_construct(CstCssPair* self, SysChar *key, CstCssValue *value) {
  self->key = sys_strdup(key);
  self->value = value;
}

CstCssPair *cst_css_pair_new_I(SysChar *key, CstCssValue *value) {
  sys_return_val_if_fail(key != NULL, NULL);
  sys_return_val_if_fail(value != NULL, NULL);

  CstCssPair *o = cst_css_pair_new();

  cst_css_pair_construct(o, key, value);

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

