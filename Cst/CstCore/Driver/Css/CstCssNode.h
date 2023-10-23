#ifndef __CST_CSS_NODE_H__
#define __CST_CSS_NODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_NODE (cst_css_node_get_type())
#define CST_CSS_NODE(o) ((CstCssNode* )sys_object_cast_check(o, CST_TYPE_CSS_NODE))
#define CST_CSS_NODE_CLASS(o) ((CstCssNodeClass *)sys_class_cast_check(o, CST_TYPE_CSS_NODE))
#define CST_CSS_NODE_GET_CLASS(o) sys_instance_get_class(o, CstCssNodeClass)

struct _CstCssNode {
  SysObject parent;

  /* <private> */
  SysChar* name;
  SysInt css_ptype;
  SysInt css_state;
  CstCssFunc set_value;
  CstCssValue *value;
};

struct _CstCssNodeClass {
  SysObjectClass parent;
};


SYS_API SysType cst_css_node_get_type(void);
SYS_API CstCssNode *cst_css_node_new(void);
SYS_API CstCssNode* cst_css_node_new_I(SysChar* name, SysInt css_ptype, SysInt css_state, CstCssFunc set_value);

SYS_API void cst_css_node_setup(void);
SYS_API void cst_css_node_teardown(void);

SYS_API const SysChar* cst_css_node_name(CstCssNode* o);
SYS_API CstCssNode* cst_css_node_lookup(const SysChar* name);
SYS_API CST_CSS_PROP_ENUM cst_css_node_get_css_ptype(const SysChar * name);
SYS_API void cst_css_node_bind_map(SysChar* name, SysInt css_type, SysInt css_state, CstCssFunc set_value);
SYS_API void cst_css_node_set_value(CstCssNode *self, CstRenderNode *node, CstLayout *layout, SysPointer user_data);
SYS_API SysBool cst_css_node_check(CstCssNode *self, CstLayout *layout);


SYS_END_DECLS

#endif

