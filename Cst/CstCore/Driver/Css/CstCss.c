#include <CstCore/Driver/Css/CstCssPrivate.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Front/CstFrontCore.h>


struct _CstCssClosure {
  SysPointer data;
  CstCssCalcFunc func;
  SysDestroyFunc free;
  SysRef ref_count;
};

struct _CstCssPair {
  SysChar *key;
  CstCssValue *value;
};

struct _CstCssGroupPrivate {
  SysPtrArray *base;
  SysPtrArray *pairs;
  SysChar *id;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstCssGroup, cst_css_group, SYS_TYPE_OBJECT);

/* CstCssPair */
void cst_css_pair_free(CstCssPair *pair) {
  sys_return_if_fail(pair != NULL);

  sys_free_N(pair->key);

  sys_clear_pointer(&pair->value, cst_css_value_free);
  sys_free_N(pair);
}

CstCssPair *cst_css_pair_new(SysChar *key, CstCssValue *value) {
  sys_return_val_if_fail(key != NULL, NULL);
  sys_return_val_if_fail(value != NULL, NULL);

  CstCssPair *pair = sys_new0_N(CstCssPair, 1);

  pair->key = sys_strdup(key);
  pair->value = value;

  return pair;
}

CstCssPair *cst_css_pair_clone(CstCssPair *pair) {
  sys_return_val_if_fail(pair != NULL, NULL);

  CstCssValue *nvalue = cst_css_value_clone(pair->value);
  CstCssPair *npair = cst_css_pair_new(pair->key, nvalue);

  return npair;
}

/* closure */
void cst_css_closure_calc(CstCssClosure *closure, CstNode *v_parent, CstNode *v_node, FRContext *cr) {
  sys_return_if_fail(v_node != NULL);

  closure->func(v_parent, v_node, cr, closure->data);
}

CstCssClosure* cst_css_closure_new(SysPointer data, CstCssCalcFunc calc, SysDestroyFunc free) {
  CstCssClosure *c = sys_new_N(CstCssClosure, 1);

  c->data = data;
  c->func = calc;
  c->free = free;

  sys_ref_count_init(c);

  return c;
}

CstCssClosure* cst_css_closure_clone(CstCssClosure *c) {
  sys_return_val_if_fail(c != NULL, NULL);

  CstCssClosure *nc = cst_css_closure_new(c->data, c->func, c->free);

  return nc;
}

void cst_css_closure_ref(CstCssClosure *c) {
  sys_return_if_fail(c != NULL);

  sys_ref_count_inc(c);
}

void cst_css_closure_unref(CstCssClosure *c) {
  sys_return_if_fail(c != NULL);

  if (!sys_ref_count_dec(c)) {
    return;
  }

  cst_css_closure_free(c);
}

void cst_css_closure_free(CstCssClosure *c) {
  sys_return_if_fail(c != NULL);

  if (c->free) {
    if (c->data != NULL) {
      c->free(c->data);
    }
  }

  sys_free_N(c);
}

/* CstCssGroup */
SysPtrArray *css_group_base_new(void) {
  SysPtrArray *base = sys_ptr_array_new_with_free_func((SysDestroyFunc)_sys_object_unref);
  return base;
}

CstCssGroup *cst_css_group_clone(CstCssGroup *self) {
  sys_return_val_if_fail(self != NULL, false);

  CstCssGroupPrivate* opriv = self->priv;
  CstCssGroup *ng = cst_css_group_new_I(opriv->id);
  CstCssGroupPrivate* npriv = ng->priv;

  SysPtrArray *ptr;

  ptr = opriv->pairs;
  for(SysUInt i = 0; i < ptr->len; i++) {
    CstCssPair *pair = cst_css_pair_clone(ptr->pdata[i]);
    sys_ptr_array_add(npriv->pairs, pair);
  }

  ptr = opriv->base;
  if (ptr != NULL) {
    npriv->base = css_group_base_new();

    for (SysUInt i = 0; i < ptr->len; i++) {
      CstCssGroup *base = cst_css_group_clone(ptr->pdata[i]);

      sys_ptr_array_add(npriv->base, base);
    }
  }

  return ng;
}

static SysBool cst_css_exists(SysPtrArray *ptr, CstCssGroup *ng) {
  if (ptr->len == 0) {
    return false;
  }

  sys_array_foreach(CstCssGroup *, cg, ptr->pdata, ptr->len) {
    CstCssGroupPrivate* npriv = cg->priv;

    if (cg == ng) {
      sys_warning_N("load duplicate global css: %s", npriv->id);
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

  CstCssGroupPrivate* priv = self->priv;

  sys_return_val_if_fail(priv->id != NULL, false);

  if (cst_css_exists(ptr, self)) {
    return false;
  }

  sys_object_ref(self);
  sys_ptr_array_add(ptr, self);

  if (priv->base == NULL || priv->base->len == 0) {
    return true;
  }

  sys_array_foreach(CstCssGroup *, cg, priv->base->pdata, priv->base->len) {
    CstCssGroupPrivate* cpriv = cg->priv;

    sys_assert(cpriv->id != NULL && "CstCssGroup id should not be null, maybe destroyed ?");

    cst_css_group_set_r(ptr, cg);
  }

  return true;
}

void cst_css_group_pair_add(CstCssGroup *self, CstCssPair *pair) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(pair != NULL);

  CstCssGroupPrivate* priv = self->priv;

  sys_ptr_array_add(priv->pairs, pair);
}

SysBool cst_css_group_set_base_r(CstCssGroup *self, CstCssGroup *ng) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(ng != NULL, false);

  CstCssGroupPrivate* priv = self->priv;

  if (priv->base == NULL) {
    priv->base = css_group_base_new();
  }

  return cst_css_group_set_r(priv->base, ng);
}

void cst_css_group_add_pair(CstCssGroup *self, SysChar *key, CstCssValue *value) {
  sys_return_if_fail(self != NULL);

  CstCssGroupPrivate* priv = self->priv;

  CstCssPair *pair = cst_css_pair_new(key, value);

  sys_ptr_array_add(priv->pairs, pair);
}

void cst_css_render_groups(CstNode *node, SysPtrArray *gs, FRContext *cr, CST_RENDER_STATE_ENUM state) {
  sys_return_if_fail(node != NULL);
  sys_return_if_fail(gs != NULL);
  sys_return_if_fail(cr != NULL);

  CstCssGroupPrivate* priv;
  SysInt g_type;
  CstCssPair *pair;
  CstCssGroup *self;

  for (SysInt i = (SysInt)(gs->len - 1); i >= 0; i--) {
    self = gs->pdata[i];

    priv = self->priv;

    if (priv->pairs->len == 0) {
      continue;
    }

    for (SysInt j = 0; j < (SysInt)priv->pairs->len; j++) {
      pair = priv->pairs->pdata[j];
      g_type = cst_css_value_get_g_type(pair->value);

      if (state != g_type) {
        continue;
      }

      // sys_debug_N("%s,%s", cst_node_get_id(node), pair->key);

      switch (state) {
        case CST_RENDER_STATE_LAYOUT:
          cst_css_value_layout(pair->value, cr, node);
          break;
        case CST_RENDER_STATE_PAINT:
          cst_css_value_paint(pair->value, cr, node);
          break;
        default:
          sys_warning_N("Not support css render state: %d.", g_type);
          break;
      }
    }
  }
}

const SysChar* cst_css_group_get_id(CstCssGroup *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstCssGroupPrivate* priv = self->priv;

  return priv->id;
}

SysPtrArray *cst_css_group_get_base(CstCssGroup *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstCssGroupPrivate* priv = self->priv;

  return priv->base;
}

/* CstCssGroup object api */
void cst_css_group_construct(SysObject *o, const SysChar *id) {

  CstCssGroup* self = CST_CSS_GROUP(o);
  CstCssGroupPrivate* priv = self->priv;

  priv->pairs = sys_ptr_array_new_with_free_func((SysDestroyFunc)cst_css_pair_free);
  priv->base = NULL;
  priv->id = sys_strdup(id);
}

CstCssGroup* cst_css_group_new(void) {
  return sys_object_new(CST_TYPE_CSS_GROUP, NULL);
}

CstCssGroup *cst_css_group_new_I(const SysChar *id) {
  sys_return_val_if_fail(id != NULL, NULL);

  CstCssGroup *o = cst_css_group_new();

  cst_css_group_construct(SYS_OBJECT(o), id);

  return o;
}

static void cst_css_group_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);
  CstCssGroup *self = CST_CSS_GROUP(o);
  CstCssGroupPrivate* priv = self->priv;

  if (priv->base != NULL) {
    sys_clear_pointer(&priv->base, sys_ptr_array_unref);
  }

  sys_clear_pointer(&priv->pairs, sys_ptr_array_unref);
  sys_free_N(priv->id);

  SYS_OBJECT_CLASS(cst_css_group_parent_class)->dispose(o);
}

static void cst_css_group_class_init(CstCssGroupClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)cst_css_group_construct;
  ocls->dispose = cst_css_group_dispose;
}

void cst_css_group_init(CstCssGroup *self) {
  self->priv = cst_css_group_get_private(self);
}

void cst_css_setup(void) {
  cst_css_value_setup();
  cst_css_env_setup();
}

void cst_css_teardown(void) {
  cst_css_value_teardown();
  cst_css_env_teardown();
}
