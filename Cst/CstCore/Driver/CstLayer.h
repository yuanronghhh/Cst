#ifndef __CST_LAYER_H__
#define __CST_LAYER_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_LAYER (cst_layer_get_type())
#define CST_LAYER(o) ((CstLayer* )sys_object_cast_check(o, CST_TYPE_LAYER))
#define CST_LAYER_CLASS(o) ((CstLayerClass *)sys_class_cast_check(o, CST_TYPE_LAYER))
#define CST_LAYER_GET_CLASS(o) sys_instance_get_class(o, CstLayerClass)

struct _CstLayer {
  SysObject parent;

  /* < private > */
  SysQueue *draw_queue;
};

struct _CstLayerClass {
  SysObjectClass parent;

  CstLayerNode* (*new_node) (CstLayer *layer, CstNode *node);
  CstLayerNode* (*realize_node) (CstLayer *layer, CstLayerNode *parent, CstNode *node, CstLayout *layout);
  CstLayerNode* (*realize_node) (CstLayer *layer, CstNodeRealizer* pass, CstRenderNode *rnode);
};

SYS_API SysType cst_layer_get_type(void);
SYS_API CstLayer *cst_layer_new(void);

CstLayerNode* cst_layer_new_node(CstLayer *o, CstNode *node);
CstLayerNode* cst_layer_realize_node (CstLayer *layer, CstNodeRealizer* pass, CstRenderNode *rnode);

void cst_layer_queue_draw_node(CstLayer *self, CstRenderNode *v_node);

SYS_END_DECLS

#endif

