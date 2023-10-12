#include <CstCore/Driver/CstPropMap.h>

SYS_DEFINE_TYPE(CstPropMap, cst_prop_map, SYS_TYPE_OBJECT);

CstPropMap* cst_prop_map_new(void) {
  return sys_object_new(CST_TYPE_PROP_MAP, NULL);
}

const SysChar* cst_prop_map_key(CstPropMap *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->key;
}

SysInt cst_prop_map_prop_data_type(CstPropMap *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->data_type;
}

/* object api */
static void cst_prop_map_init(CstPropMap *self) {
}

static void cst_prop_map_construct(CstPropMap* self, const SysChar *key, CST_PROP_VALUE_ENUM data_type) {
  self->key = sys_strdup(key);
  self->data_type = data_type;
}

SysInt cst_prop_map_parse_type(const SysChar *key, const SysChar* type_str) {
  if (sys_str_equal(type_str, "string")) {

    return SYS_STRING;
  } else if(sys_str_equal(type_str, "bool")) {

    return SYS_BOOL;
  } else if(sys_str_equal(type_str, "function")) {

    return SYS_POINTER;
  } else {

    sys_warning_N("Not found prop type: %s", type_str);
    return -1;
  }
}

CstPropMap* cst_prop_map_new_I(const SysChar *key, CST_PROP_VALUE_ENUM data_type) {
  CstPropMap* map = cst_prop_map_new();

  cst_prop_map_construct(map, key, data_type);

  return map;
}

static void cst_prop_map_dispose(SysObject* o) {
  CstPropMap *self = CST_PROP_MAP(o);
  sys_free_N(self->key);

  SYS_OBJECT_CLASS(cst_prop_map_parent_class)->dispose(o);
}

static void cst_prop_map_class_init(CstPropMapClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_prop_map_dispose;
}
