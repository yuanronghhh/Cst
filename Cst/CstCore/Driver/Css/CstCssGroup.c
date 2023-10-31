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

SysObject *cst_css_group_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, false);

  SysObject *n = SYS_OBJECT_CLASS(cst_css_group_parent_class)->dclone(o);

  CstCssGroup *nself = CST_CSS_GROUP(n);
  CstCssGroup *oself = CST_CSS_GROUP(o);

  nself->id = sys_strdup(oself->id);

  for(SysUInt i = 0; i < oself->pairs->len; i++) {
    CstCssPair *pair = (CstCssPair *)sys_object_dclone(oself->pairs->pdata[i]);
    sys_ptr_array_add(nself->pairs, pair);
  }

  if (oself->base != NULL) {
    nself->base = css_group_base_new();

    for (SysUInt i = 0; i < oself->base->len; i++) {
      CstCssGroup *base = (CstCssGroup *)sys_object_dclone(oself->base->pdata[i]);

      sys_ptr_array_add(nself->base, base);
    }
  }

  return n;
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

SysBool cst_css_group_set_by_id(SysPtrArray *ptr, FREnv *env, const SysChar *id) {
  sys_return_val_if_fail(ptr != NULL, false);
  sys_return_val_if_fail(id != NULL, false);

  CstCssGroup *self = fr_env_get_r(env, id);
  if (self == NULL) {
    sys_warning_N("set css by id failed: %s", id);
    return false;
  }

  return cst_css_group_set_r(ptr, self);
}

SysBool cst_css_group_set_r(SysPtrArray *ptr, CstCssGroup *ng) {
  sys_return_val_if_fail(ptr != NULL, false);
  sys_return_val_if_fail(ng != NULL, false);
  sys_return_val_if_fail(ng->id != NULL, false);

  if (cst_css_exists(ptr, ng)) {
    return false;
  }

  sys_object_ref(ng);
  sys_ptr_array_add(ptr, ng);

  if (ng->base == NULL || ng->base->len == 0) {
    return true;
  }

  for (int i = 0; i < (int)ng->base->len; i++) {
    CstCssGroup* cg = ng->base->pdata[i];

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

void cst_css_group_add_pair(CstCssGroup *self, CstCssPair *pair) {
  sys_return_if_fail(self != NULL);

  sys_ptr_array_add(self->pairs, pair);
}

const SysChar* cst_css_group_get_id(CstCssGroup *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->id;
}

SysPtrArray *cst_css_group_get_base(CstCssGroup *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->base;
}

/* cst_css_group_list */
void cst_css_group_list_render(SysPtrArray *gs, CstRenderNode *render_node, CstLayout *layout) {
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

SysBool cst_css_group_list_exists(SysPtrArray *css_list, CstCssGroup *g) {
  sys_return_val_if_fail(css_list != NULL, false);

  if (css_list->len == 0) { return false; }

  for (SysInt i = 0; i < (SysInt)css_list->len; i++) {
    CstCssGroup * cg = css_list->pdata[i];

    if (cg == g) {

      return true;
    }
  }

  return false;
}

/* object api */
static void cst_css_group_construct(CstCssGroup* self, const SysChar *id) {
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
  sys_clear_pointer(&self->id, sys_free);

  SYS_OBJECT_CLASS(cst_css_group_parent_class)->dispose(o);
}

static void cst_css_group_class_init(CstCssGroupClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_group_dispose;
  ocls->dclone = cst_css_group_dclone_i;
}

void cst_css_group_init(CstCssGroup *self) {

  self->pairs = sys_ptr_array_new_with_free_func((SysDestroyFunc)_sys_object_unref);
}

