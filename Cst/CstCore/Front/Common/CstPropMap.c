#include <CstCore/Front/Common/CstPropMap.h>


struct _CstPropMapPrivate {
  SysChar *key;
  SysValue *v_default;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstPropMap, cst_prop_map, SYS_TYPE_OBJECT);

CstPropMap* cst_prop_map_new(void) {
  return sys_object_new(CST_TYPE_PROP_MAP, NULL);
}

const SysChar* cst_prop_map_key(CstPropMap *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstPropMapPrivate* priv = self->priv;

  return priv->key;
}

SysInt cst_prop_map_prop_data_type(CstPropMap *self) {
  sys_return_val_if_fail(self != NULL, -1);

  CstPropMapPrivate* priv = self->priv;

  return sys_value_data_type(priv->v_default);
}

/* object api */
static void cst_prop_map_init(CstPropMap *self) {
  self->priv = cst_prop_map_get_private(self);
}

static void cst_prop_map_construct(SysObject *o, const SysChar *key, SysValue *v_default) {
  SYS_OBJECT_CLASS(cst_prop_map_parent_class)->construct(o);

  CstPropMap *self = CST_PROP_MAP(o);
  CstPropMapPrivate* priv = self->priv;

  priv->key = sys_strdup(key);
  priv->v_default = v_default;
}

CstPropMap* cst_prop_map_new_I(const SysChar *key, SysValue *v_default) {
  CstPropMap* map = cst_prop_map_new();

  cst_prop_map_construct(SYS_OBJECT(map), key, v_default);

  return map;
}

static void cst_prop_map_dispose(SysObject* o) {
  CstPropMap *self = CST_PROP_MAP(o);
  CstPropMapPrivate* priv = self->priv;

  sys_free_N(priv->key);
  if(priv->v_default) {
    sys_clear_pointer(&priv->v_default, sys_value_unref);
  }

  SYS_OBJECT_CLASS(cst_prop_map_parent_class)->dispose(o);
}

static void cst_prop_map_class_init(CstPropMapClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)cst_prop_map_construct;
  ocls->dispose = cst_prop_map_dispose;
}
