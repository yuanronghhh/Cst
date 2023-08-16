#include <CstCore/Front/Common/CstPropPair.h>


struct _CstPropPair {
  SysChar *key;
  CstPropValue *value;
};

void cst_prop_pair_free(CstPropPair *pair) {
  sys_return_if_fail(pair != NULL);

  sys_free_N(pair->key);
  sys_clear_pointer(&pair->value, sys_value_free);
  sys_free_N(pair);
}

CstPropPair* cst_prop_pair_new(SysChar *key, SysValue *value) {
  sys_return_val_if_fail(key != NULL, NULL);
  sys_return_val_if_fail(value != NULL, NULL);

  CstPropPair *pair = sys_new0_N(CstPropPair, 1);

  pair->key = sys_strdup(key);
  pair->value = value;

  return pair;
}

SysValue * cst_prop_pair_value(CstPropPair *pair) {
  sys_return_val_if_fail(pair != NULL, NULL);

  return pair->value;
}

SysChar * cst_prop_pair_key(CstPropPair *pair) {
  sys_return_val_if_fail(pair != NULL, NULL);

  return pair->key;
}