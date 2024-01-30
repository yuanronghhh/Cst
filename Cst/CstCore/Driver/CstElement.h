#ifndef __CST_ELEMENT_H__
#define __CST_ELEMENT_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_ELEMENT_TYPE (cst_element_get_type())
#define CST_ELEMENT(o) ((CstElement* )sys_object_cast_check(o, CST_ELEMENT_TYPE))
#define CST_ELEMENT_CLASS(o) ((CstElementClass *)sys_class_cast_check(o, CST_ELEMENT_TYPE))
#define CST_ELEMENT_GET_CLASS(o) sys_instance_get_class(o, CstElementClass)


struct _CstElementClass {
  SysObjectClass parent;
};

struct _CstElement {
  SysObject unowned;

  /* <private> */
  CstNode *node;
  CstRenderNode *rnode;
  CstLayerNode *lnode;
};

SYS_API SysType cst_element_get_type(void);
SYS_API CstElement *cst_element_new_I(void);

SYS_END_DECLS

#endif
