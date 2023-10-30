#ifndef __CST_NODE_REALIZER_H__
#define __CST_NODE_REALIZER_H__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_NODE_REALIZER (cst_node_realizer_get_type())
#define CST_NODE_REALIZER(o) ((CstNodeRealizer* )sys_object_cast_check(o, CST_TYPE_NODE_REALIZER))
#define CST_NODE_REALIZER_CLASS(o) ((CstNodeRealizerClass *)sys_class_cast_check(o, CST_TYPE_NODE_REALIZER))
#define CST_NODE_REALIZER_GET_CLASS(o) sys_instance_get_class(o, CstNodeRealizerClass)

struct _CstNodeRealizer {
  SysObject unowned;

  /* <private> */
  CstLayerNode *parent;
  CstModule *v_module;
  CstComNode *com_node;
};

struct _CstNodeRealizerClass {
  SysObjectClass parent;
};

CstNodeRealizer* cst_node_realizer_new(void);
CstNodeRealizer* cst_node_realizer_new_I(CstLayerNode *parent, CstModule *v_module, CstComNode *com_node);
CstLayerNode *cst_node_realizer_realize(CstNodeRealizer* realizer, CstNode *node);

SYS_END_DECLS

#endif
