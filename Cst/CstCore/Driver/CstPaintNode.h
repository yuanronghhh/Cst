#ifndef __CST_PAINT_NODE__
#define __CST_PAINT_NODE__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_PAINT_NODE (cst_paint_node_get_type())
#define CST_PAINT_NODE(o) ((CstPaintNode* )sys_object_cast_check(o, CST_TYPE_PAINT_NODE))
#define CST_PAINT_NODE_CLASS(o) ((CstPaintNodeClass *)sys_class_cast_check(o, CST_TYPE_PAINT_NODE))
#define CST_PAINT_NODE_GET_CLASS(o) sys_instance_get_class(o, CstPaintNodeClass)

struct _CstPaintNode {
  SysObject parent;

  /* <private> */
  SysBool is_visible;
};

struct _CstPaintNodeClass {
  SysObjectClass parent;
};

SysType cst_paint_node_get_type(void);
CstPaintNode *cst_paint_node_new(void);
CstPaintNode *cst_paint_node_new_I(void);

SysBool cst_paint_node_is_visible(CstPaintNode* self);

SYS_END_DECLS

#endif

