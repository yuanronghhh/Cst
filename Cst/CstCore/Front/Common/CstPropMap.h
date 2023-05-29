#ifndef __CST_PROP_MAP__
#define __CST_PROP_MAP__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS


#define CST_TYPE_PROP_MAP (cst_prop_map_get_type())
#define CST_PROP_MAP(o) ((CstPropMap* )sys_object_cast_check(o, CST_TYPE_PROP_MAP))
#define CST_PROP_MAP_CLASS(o) ((CstPropMapClass *)sys_class_cast_check(o, CST_TYPE_PROP_MAP))
#define CST_PROP_MAP_GET_CLASS(o) sys_instance_get_class(o, CstPropMapClass)


struct _CstPropMap {
  SysObject parent;

  CstPropMapPrivate *priv;
};

struct _CstPropMapClass {
  SysObjectClass parent;
};

SysType cst_prop_map_get_type(void);
CstPropMap* cst_prop_map_new(void);
const SysChar * cst_prop_map_key(CstPropMap * self);
SysInt cst_prop_map_prop_data_type(CstPropMap * self);
CstPropMap * cst_prop_map_new_I(const SysChar * key, SysValue * v_default);

SYS_END_DECLS

#endif

