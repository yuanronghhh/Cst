#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Css/CstCssValue.h>


SYS_DEFINE_TYPE(CstCssGroup, cst_css_group, SYS_TYPE_OBJECT);


CstCssGroup *cst_css_group_clone(CstCssGroup *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  CstCssValue *nvalue = cst_css_value_clone(o->value);
  CstCssGroup *ngroup = cst_css_group_new_I(o->key, nvalue);

  return ngroup;
}

/* object api */
static void cst_css_group_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstCssGroup *self = CST_CSS_GROUP(o);

  sys_clear_pointer(&self->key, sys_free);
  sys_clear_pointer(&self->value, (SysDestroyFunc)_sys_object_unref);

  SYS_OBJECT_CLASS(cst_css_group_parent_class)->dispose(o);
}

static void cst_css_group_construct(CstCssGroup* self, SysChar *key, CstCssValue *value) {
  self->key = sys_strdup(key);
  self->value = value;
}

CstCssGroup *cst_css_group_new_I(SysChar *key, CstCssValue *value) {
  sys_return_val_if_fail(key != NULL, NULL);
  sys_return_val_if_fail(value != NULL, NULL);

  CstCssGroup *o = cst_css_group_new();

  cst_css_group_construct(o, key, value);

  return o;
}

CstCssGroup *cst_css_group_new(void) {
  return sys_object_new(CST_TYPE_CSS_GROUP, NULL);
}

static void cst_css_group_class_init(CstCssGroupClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_group_dispose;
}

static void cst_css_group_init(CstCssGroup *self) {
}

