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


SysType cst_css_node_get_type(void);
CstCssNode *cst_css_node_new(void);
CstCssNode* cst_css_node_new_I(SysChar* name, SysInt css_ptype, SysInt css_state, CstCssFunc set_value);

void cst_css_node_setup(void);
void cst_css_node_teardown(void);

const SysChar* cst_css_node_name(CstCssNode* o);
CstCssNode* cst_css_node_lookup(const SysChar* name);
void cst_css_node_bind_map(SysChar* name, SysInt css_type, SysInt css_state, CstCssFunc set_value);
void cst_css_node_set_value(CstCssNode *self, CstRenderNode *node, CstLayout *layout, SysPointer user_data);
SysBool cst_css_node_check(CstCssNode *self, CstLayout *layout);

void cst_css_node_set_css_ptype(CstCssNode *self, SysInt css_ptype);
SysInt cst_css_node_get_css_ptype(CstCssNode *self);

SYS_END_DECLS

#endif

