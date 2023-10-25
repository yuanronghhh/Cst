#ifndef __CST_VALUE_MAP__
#define __CST_VALUE_MAP__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS


#define CST_TYPE_PROP_MAP (cst_value_map_get_type())
#define CST_VALUE_MAP(o) ((CstValueMap* )sys_object_cast_check(o, CST_TYPE_PROP_MAP))
#define CST_VALUE_MAP_CLASS(o) ((CstValueMapClass *)sys_class_cast_check(o, CST_TYPE_PROP_MAP))
#define CST_VALUE_MAP_GET_CLASS(o) sys_instance_get_class(o, CstValueMapClass)


struct _CstValueMap {
  SysObject parent;

  /* <private> */
  SysChar* key;

  /* SYS_VALUE_ENUM */
  SysInt data_type;
};

struct _CstValueMapClass {
  SysObjectClass parent;
};

SysType cst_value_map_get_type(void);
CstValueMap* cst_value_map_new(void);
const SysChar * cst_value_map_key(CstValueMap * self);
SysInt cst_value_map_prop_data_type(CstValueMap * self);
SysInt cst_value_map_parse_type(const SysChar* type_str);
CstValueMap * cst_value_map_new_I(const SysChar * key, SYS_VALUE_ENUM data_type);

SYS_END_DECLS

#endif

