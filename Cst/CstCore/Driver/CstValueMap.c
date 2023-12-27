#include <CstCore/Driver/CstValueMap.h>


SYS_DEFINE_TYPE(CstValueMap, cst_value_map, SYS_TYPE_OBJECT);


CstValueMap* cst_value_map_new(void) {
  return sys_object_new(CST_TYPE_VALUE_MAP, NULL);
}

const SysChar* cst_value_map_key(CstValueMap *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->key;
}

SysInt cst_value_map_prop_data_type(CstValueMap *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->data_type;
}

FREnv *cst_value_map_new_env(FREnv *parent) {
  SysHashTable *ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, (SysDestroyFunc)_sys_object_unref);

  return fr_env_new_I(ht, parent);
}

/* object api */
static void cst_value_map_init(CstValueMap *self) {
}

static void cst_value_map_construct(CstValueMap* self, const SysChar *key, SYS_VALUE_ENUM data_type) {
  self->key = sys_strdup(key);
  self->data_type = data_type;
}

SysInt cst_value_map_parse_type(const SysChar* type_str) {
  sys_return_val_if_fail(type_str != NULL, -1);

  if (sys_str_equal(type_str, "string")) {

    return SYS_VALUE_STRING;
  } else if(sys_str_equal(type_str, "null")) {

    return SYS_VALUE_NULL;
  } else if(sys_str_equal(type_str, "double")) {

    return SYS_VALUE_DOUBLE;
  } else if(sys_str_equal(type_str, "int")) {

    return SYS_VALUE_INT;
  } else if(sys_str_equal(type_str, "bool")) {

    return SYS_VALUE_BOOL;
  } else if(sys_str_equal(type_str, "object")) {

    return SYS_VALUE_POINTER;
  } else if(sys_str_equal(type_str, "function")) {

    return SYS_VALUE_POINTER;
  } else {

    sys_warning_N("Not found prop type: %s", type_str);
    return -1;
  }
}

CstValueMap * cst_value_map_new_I(const SysChar * key, SYS_VALUE_ENUM data_type) {
  CstValueMap* map = cst_value_map_new();

  cst_value_map_construct(map, key, data_type);

  return map;
}

static void cst_value_map_dispose(SysObject* o) {
  CstValueMap *self = CST_VALUE_MAP(o);
  
  sys_clear_pointer(&self->key, sys_free);

  SYS_OBJECT_CLASS(cst_value_map_parent_class)->dispose(o);
}

static void cst_value_map_class_init(CstValueMapClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_value_map_dispose;
}
