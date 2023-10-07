#ifndef __CST_CSS_GROUP_H__
#define __CST_CSS_GROUP_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_GROUP (cst_css_group_get_type())
#define CST_CSS_GROUP(o) ((CstCssGroup* )sys_object_cast_check(o, CST_TYPE_CSS_GROUP))
#define CST_CSS_GROUP_CLASS(o) ((CstCssGroupClass *)sys_class_cast_check(o, CST_TYPE_CSS_GROUP))
#define CST_CSS_GROUP_GET_CLASS(o) sys_instance_get_class(o, CstCssGroupClass)

struct _CstCssGroup {
  SysObject parent;

  /* < private > */
  SysPtrArray* base;
  SysPtrArray* pairs;
  SysChar* id;
};

struct _CstCssGroupClass {
  SysObjectClass parent;
};

SYS_API SysType cst_css_group_get_type(void);
SYS_API CstCssGroup *cst_css_group_new(void);
SYS_API CstCssGroup *cst_css_group_clone(CstCssGroup *o);
SYS_API CstCssGroup *cst_css_group_new_I(const SysChar *id);

SYS_API SysPtrArray* cst_css_group_get_base(CstCssGroup *g);
SYS_API void cst_css_group_pair_add(CstCssGroup *g, CstCssPair *pair);
SYS_API SysBool cst_css_group_set_base_r(CstCssGroup *g, CstCssGroup *ng);
SYS_API SysBool cst_css_group_set_r(SysPtrArray *ptr, CstCssGroup *g);
SYS_API SysBool cst_css_group_set_by_id(SysPtrArray *ptr, CstCssEnv *env, const SysChar *id);
SYS_API const SysChar* cst_css_group_get_id(CstCssGroup *g);
SYS_API void cst_css_group_add_pair(CstCssGroup *g, SysChar *key, CstCssValue *value);
SYS_API void cst_css_render_groups(SysPtrArray *gs, CstRenderNode *render_node, CstLayout *layout);


SYS_END_DECLS

#endif

