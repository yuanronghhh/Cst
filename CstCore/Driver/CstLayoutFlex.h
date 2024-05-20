#ifndef __CST_LAYOUT_FLEX__
#define __CST_LAYOUT_FLEX__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT_FLEX (cst_layout_flex_get_type())
#define CST_LAYOUT_FLEX(o) ((CstLayoutFlex* )sys_object_cast_check(o, CST_TYPE_LAYOUT_FLEX))
#define CST_LAYOUT_FLEX_CLASS(o) ((CstLayoutFlexClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT_FLEX))
#define CST_LAYOUT_FLEX_GET_CLASS(o) sys_instance_get_class(o, CstLayoutFlexClass)

struct _CstLayoutFlex {
  CstRenderContext parent;

  /* <private> */
};

struct _CstLayoutFlexClass {
  CstRenderContextClass parent;
};

SysType cst_layout_flex_get_type(void);
CstRenderContext *cst_layout_flex_new(void);

void cst_layout_flex_layout(CstLayoutFlex* self, CstRenderNode *render_node);

SYS_END_DECLS

#endif

