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
  SysHArray* base;
  SysHArray* pairs;
  SysChar* id;
};

struct _CstCssGroupClass {
  SysObjectClass parent;
};

SysType cst_css_group_get_type(void);
CstCssGroup *cst_css_group_new(void);
CstCssGroup *cst_css_group_new_I(const SysChar *id);

SysHArray* cst_css_group_get_base(CstCssGroup *g);
void cst_css_group_pair_add(CstCssGroup *g, CstCssPair *pair);
SysBool cst_css_group_set_base_r(CstCssGroup *g, CstCssGroup *ng);
SysBool cst_css_group_set_r(SysHArray *ptr, CstCssGroup *g);
SysBool cst_css_group_set_by_id(SysHArray *ptr, FREnv *env, const SysChar *id);
const SysChar* cst_css_group_get_id(CstCssGroup *g);
void cst_css_group_add_pair(CstCssGroup *g, CstCssPair *pair);

SysHArray *cst_css_group_list_new(void);
void cst_css_group_list_render(SysHArray *gs, CstRenderNode *render_node, CstLayout *layout);
SysBool cst_css_group_list_exists(SysHArray *css_list, CstCssGroup *g);


SYS_END_DECLS

#endif

