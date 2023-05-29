#include <CstCore/Front/C/CstCComponent.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstManager.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/Css/CstCss.h>


struct _CstComponentPrivate {
  CstModule *v_module;

  SysChar *id;

  /* ComStyle : CstCssGroup */
  CstCssEnv *style_env;

  /* ComProps: CstPropMap */
  FREnv *prop_maps_env;

  CstNode *layout_node;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstComponent, cst_component, FR_TYPE_ENV);

static const SysChar* COMPONENT_BODY_NAMES[] = {
  "Data", "Component", "Layout", "Style", "Props"
};

static const SysChar* CST_COMPONENT_PROP_NAMES[] = {
  "id", "base"
};

CST_COMPONENT_BODY_ENUM cst_component_body_get_by_name(const SysChar * name) {
  return fr_get_type_by_name(COMPONENT_BODY_NAMES, ARRAY_SIZE(COMPONENT_BODY_NAMES), name);
}

CST_COMPONENT_PROP_ENUM cst_component_prop_get_by_name(const SysChar * name) {
  return fr_get_type_by_name(CST_COMPONENT_PROP_NAMES, ARRAY_SIZE(CST_COMPONENT_PROP_NAMES), name);
}

CstComponent* cst_component_new(void) {
  return sys_object_new(CST_TYPE_COMPONENT, NULL);
}

void cst_component_set_layout_node(CstComponent *self, CstNode *node) {
  sys_return_if_fail(self != NULL);

  CstComponentPrivate* priv = self->priv;

  priv->layout_node = node;
}

CstNode *cst_component_get_layout_node(CstComponent *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstComponentPrivate* priv = self->priv;

  return priv->layout_node;
}

void cst_component_set_id(CstComponent* self, const SysChar *id) {
  sys_return_if_fail(self != NULL);

  CstComponentPrivate* priv = self->priv;

  sys_assert(priv->id == NULL);

  priv->id = sys_strdup(id);
}

void cst_component_print(CstComponent* self) {
  sys_return_if_fail(self != NULL);

  CstComponentPrivate* priv = self->priv;

  cst_node_print_node(priv->layout_node);
}

const SysChar* cst_component_get_id(CstComponent* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstComponentPrivate* priv = self->priv;

  return priv->id;
}

void cst_component_set_css(CstComponent* self, CstCssGroup *g) {
  sys_return_if_fail(self != NULL);

  CstComponentPrivate* priv = self->priv;

  cst_css_env_set(priv->style_env, (SysPointer)cst_css_group_get_id(g), g);
}

SysBool cst_component_remove_css(CstComponent* self, CstCssGroup *g) {
  sys_return_val_if_fail(self != NULL, false);

  CstComponentPrivate* priv = self->priv;

  return cst_css_env_remove(priv->style_env, cst_css_group_get_id(g));
}

CstCssEnv *cst_component_get_css_env(CstComponent* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstComponentPrivate* priv = self->priv;

  return priv->style_env;
}

CstCssGroup *cst_component_get_css(CstComponent* self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstComponentPrivate* priv = self->priv;

  return cst_css_env_get(priv->style_env, key);
}

CstCssGroup *cst_component_get_css_r(CstComponent* self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(key != NULL, NULL);

  CstComponentPrivate* priv = self->priv;

  return cst_css_env_get_r(priv->style_env, key);
}

SysPointer cst_component_get_function(CstComponent *self, const SysChar *func_name) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(func_name != NULL, false);

  CstComponentPrivate *priv = self->priv;

  return cst_module_get_function(priv->v_module, func_name);
}

void cst_component_set_function(CstComponent *self, const SysChar *func_name, SysPointer func) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(func_name != NULL);
  sys_return_if_fail(sys_strneq(func_name, FR_FUNC_PREFIX, 2));
  sys_return_if_fail(func != NULL);

  CstComponentPrivate *priv = self->priv;

  cst_module_set_function(priv->v_module, func_name, func);
}

CstPropMap *cst_component_get_props_map(CstComponent *self, const SysChar *key) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstComponentPrivate *priv = self->priv;

  return fr_env_get_r(priv->prop_maps_env, key);
}

void cst_component_set_props_map(CstComponent *self, CstPropMap *map) {
  sys_return_if_fail(self != NULL);

  CstComponentPrivate *priv = self->priv;

  fr_env_set(priv->prop_maps_env, cst_prop_map_key(map), (SysPointer)map);
}

void cst_component_realize_full(CstModule *v_module, CstComponent *self, CstNode *v_parent, CstComNode *ncomp_node, CstRender *v_render) {
  sys_return_if_fail(self != NULL);

  CstComponentPrivate *priv = self->priv;

  cst_node_realize_root(v_module, ncomp_node, priv->layout_node, v_parent, v_render);
}

void cst_component_realize(CstModule *v_module, CstComponent *self, CstNode *v_parent, CstRender *v_render) {
  cst_component_realize_full(v_module, self, v_parent, NULL, v_render);
}

/* sys object api */
void cst_component_construct(CstComponent *self, CstModule *v_module, CstComponent *v_parent) {
  sys_return_if_fail(self != NULL);

  CstComponentClass *cls = CST_COMPONENT_GET_CLASS(self);

  if(cls->construct) {
    cls->construct(self, v_module, v_parent);
  }
}

void cst_component_bind_parent(CstComponent *self, CstComponent *v_parent) {
  CstComponentPrivate *priv = self->priv;
  CstComponentPrivate *ppriv = v_parent->priv;

  fr_env_set_parent(priv->prop_maps_env, ppriv->prop_maps_env);
  fr_env_set_parent(FR_ENV(priv->style_env), FR_ENV(ppriv->style_env));
}

static void cst_component_construct_i(CstComponent *self, CstModule *v_module, CstComponent *v_parent) {
  sys_return_if_fail(v_module != NULL);
  SysHashTable *ht;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  SYS_OBJECT_CLASS(cst_component_parent_class)->construct(SYS_OBJECT(self), ht, FR_ENV(v_parent));

  CstComponentPrivate *priv = self->priv;

  ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);
  priv->prop_maps_env = fr_env_new_I(ht, NULL);

  priv->style_env = cst_css_env_new_I(NULL);
  priv->v_module = v_module;

  if (v_parent) {
  }

  priv->layout_node = NULL;
}

static void cst_component_class_init(CstComponentClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = cst_component_construct_i;

  ocls->construct = (SysObjectFunc)cst_component_construct_i;
  ocls->dispose = cst_component_dispose;
}

static void cst_component_dispose(SysObject* o) {
  CstComponent *self = CST_COMPONENT(o);
  CstComponentPrivate* priv = self->priv;

  sys_object_unref(priv->style_env);
  sys_object_unref(priv->prop_maps_env);

  cst_node_unlink_node_r(priv->layout_node);

  sys_free_N(priv->id);

  SYS_OBJECT_CLASS(cst_component_parent_class)->dispose(o);
}

static void cst_component_init(CstComponent *self) {
  self->priv = cst_component_get_private(self);
}
