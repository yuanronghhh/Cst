#ifndef __CST_CSS_PAIR_H__
#define __CST_CSS_PAIR_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_PAIR (cst_css_pair_get_type())
#define CST_CSS_PAIR(o) ((CstCssPair* )sys_object_cast_check(o, CST_TYPE_CSS_PAIR))
#define CST_CSS_PAIR_CLASS(o) ((CstCssPairClass *)sys_class_cast_check(o, CST_TYPE_CSS_PAIR))
#define CST_CSS_PAIR_GET_CLASS(o) sys_instance_get_class(o, CstCssPairClass)

struct _CstCssPair {
  SysObject parent;

  /* < private > */

  CstCssNode *css_node;
  CstCssValue *value;
};

struct _CstCssPairClass {
  SysObjectClass parent;

  CstCssValue *(*parse) (CstCssParser *ps);
};

SYS_API SysType cst_css_pair_get_type(void);
SYS_API CstCssPair* cst_css_pair_new_I(CstCssNode* node, CstCssValue* value);
SYS_API CstCssPair* cst_css_pair_new_by_key(const SysChar* key);
SYS_API CstCssPair *cst_css_pair_new(void);
SYS_API CstCssPair *cst_css_pair_clone(CstCssPair *o);
SYS_API void cst_css_pair_calc(CstCssPair *pair, CstRenderNode *render_node, CstLayout *layout);

SYS_API void cst_css_pair_setup(void);
SYS_API void cst_css_pair_teardown(void);

const SysChar* cst_css_node_name(CstCssNode* o);
SysBool cst_css_node_check(CstCssNode* self, SysInt state);
CstCssNode* cst_css_node_lookup(const SysChar* name);


SYS_END_DECLS

#endif

