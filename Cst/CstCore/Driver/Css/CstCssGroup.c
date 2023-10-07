#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Driver/Css/CstCssEnv.h>

SYS_DEFINE_TYPE(CstCssGroup, cst_css_group, SYS_TYPE_OBJECT);

/* CstCssGroup */
SysPtrArray *css_group_base_new(void) {
  SysPtrArray *base = sys_ptr_array_new_with_free_func((SysDestroyFunc)_sys_object_unref);
  return base;
}

CstCssGroup *cst_css_group_clone(CstCssGroup *oself) {
  sys_return_val_if_fail(oself != NULL, false);

  CstCssGroup *nself = cst_css_group_new_I(oself->id);
  SysPtrArray *ptr = oself->pairs;

  for(SysUInt i = 0; i < ptr->len; i++) {
    CstCssPair *pair = cst_css_pair_clone(ptr->pdata[i]);
    sys_ptr_array_add(nself->pairs, pair);
  }

  ptr = oself->base;
  if (ptr != NULL) {
    nself->base = css_group_base_new();

    for (SysUInt i = 0; i < ptr->len; i++) {
      CstCssGroup *base = cst_css_group_clone(ptr->pdata[i]);

      sys_ptr_array_add(nself->base, base);
    }
  }

  return nself;
}

static SysBool cst_css_exists(SysPtrArray *ptr, CstCssGroup *ng) {
  if (ptr->len == 0) {
    return false;
  }

  for(SysInt i = 0; i < (SysInt)ptr->len; i++) {
    CstCssGroup * cg = ptr->pdata[i];

    if (cg == ng) {
      sys_warning_N("load duplicate global css: %s", cg->id);
      return true;
    }
  }

  return false;
}

SysBool cst_css_group_set_by_id(SysPtrArray *ptr, CstCssEnv *env, const SysChar *id) {
  sys_return_val_if_fail(ptr != NULL, false);
  sys_return_val_if_fail(id != NULL, false);

  CstCssGroup *self = cst_css_env_get_r(env, id);
  if (self == NULL) {
    sys_warning_N("set css by id failed: %s", id);
    return false;
  }

  return cst_css_group_set_r(ptr, self);
}

SysBool cst_css_group_set_r(SysPtrArray *ptr, CstCssGroup *self) {
  sys_return_val_if_fail(ptr != NULL, false);
  sys_return_val_if_fail(self != NULL, false);

  sys_return_val_if_fail(self->id != NULL, false);

  if (cst_css_exists(ptr, self)) {
    return false;
  }

  sys_object_ref(self);
  sys_ptr_array_add(ptr, self);

  if (self->base == NULL || self->base->len == 0) {
    return true;
  }

  for (int i = 0; i < (int)self->base->len; i++) {
    CstCssGroup* cg = self->base->pdata[i];

    sys_assert(cg->id != NULL && "CstCssGroup id should not be null, maybe destroyed ?");

    cst_css_group_set_r(ptr, cg);
  }

  return true;
}

void cst_css_group_pair_add(CstCssGroup *self, CstCssPair *pair) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(pair != NULL);

  sys_ptr_array_add(self->pairs, pair);
}

SysBool cst_css_group_set_base_r(CstCssGroup *self, CstCssGroup *ng) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(ng != NULL, false);

  if (self->base == NULL) {
    self->base = css_group_base_new();
  }

  return cst_css_group_set_r(self->base, ng);
}

void cst_css_group_add_pair(CstCssGroup *self, SysChar *key, CstCssValue *value) {
  sys_return_if_fail(self != NULL);

  CstCssPair *pair = cst_css_pair_new_I(key, value);

  sys_ptr_array_add(self->pairs, pair);
}

void cst_css_render_groups(SysPtrArray *gs, CstRenderNode *render_node, CstLayout *layout) {
  sys_return_if_fail(render_node != NULL);
  sys_return_if_fail(gs != NULL);

  CstCssPair *pair;
  CstCssGroup *self;

  for (SysInt i = (SysInt)(gs->len - 1); i >= 0; i--) {
    self = gs->pdata[i];

    if (self->pairs->len == 0) {
      continue;
    }

    for (SysInt j = 0; j < (SysInt)self->pairs->len; j++) {
      pair = self->pairs->pdata[j];
      cst_css_pair_calc(pair, render_node, layout);
    }
  }
}

const SysChar* cst_css_group_get_id(CstCssGroup *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

SysPtrArray *cst_css_group_get_base(CstCssGroup *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->base;
}

/* object api */
static void cst_css_group_construct(CstCssGroup* self, const SysChar *id) {
  self->pairs = sys_ptr_array_new_with_free_func((SysDestroyFunc)_sys_object_unref);
  self->base = NULL;
  self->id = sys_strdup(id);
}

CstCssGroup* cst_css_group_new(void) {
  return sys_object_new(CST_TYPE_CSS_GROUP, NULL);
}

CstCssGroup *cst_css_group_new_I(const SysChar *id) {
  sys_return_val_if_fail(id != NULL, NULL);

  CstCssGroup *o = cst_css_group_new();

  cst_css_group_construct(o, id);

  return o;
}

static void cst_css_group_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstCssGroup *self = CST_CSS_GROUP(o);

  if (self->base != NULL) {
    sys_clear_pointer(&self->base, sys_ptr_array_unref);
  }

  sys_clear_pointer(&self->pairs, sys_ptr_array_unref);
  sys_free_N(self->id);

  SYS_OBJECT_CLASS(cst_css_group_parent_class)->dispose(o);
}

static void cst_css_group_class_init(CstCssGroupClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_group_dispose;
}

void cst_css_group_init(CstCssGroup *self) {
}

