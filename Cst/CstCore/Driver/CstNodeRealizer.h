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
  CstLayerNode *v_parent;
  CstModule *v_module;
  CstComNode *com_node;
};

struct _CstNodeRealizerClass {
  SysObjectClass parent;
};

CstNodeRealizer* cst_node_realizer_new(void);
CstNodeRealizer* cst_node_realizer_new_I(CstLayerNode *parent, CstModule *v_module, CstComNode *com_node);
CstLayerNode *cst_node_realizer_realize(CstNodeRealizer* realizer, CstNode *node);

void cst_node_realizer_set_v_module(CstNodeRealizer *self, CstModule * v_module);
CstModule * cst_node_realizer_get_v_module(CstNodeRealizer *self);

void cst_node_realizer_set_v_parent(CstNodeRealizer *self, CstLayerNode * v_parent);
CstLayerNode * cst_node_realizer_get_v_parent(CstNodeRealizer *self);

SYS_END_DECLS

#endif
