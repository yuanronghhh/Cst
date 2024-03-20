#ifndef __CST_I_LAYER__
#define __CST_I_LAYER__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_I_LAYER (cst_i_layer_get_type())
#define CST_I_LAYER(o) ((CstILayer* )sys_object_cast_check(o, CST_TYPE_I_LAYER))
#define CST_I_LAYER_GET_IFACE(o) ((CstILayerInterface *)SYS_TYPE_GET_INTERFACE(o, CST_TYPE_I_LAYER))

typedef struct _CstILayer CstILayer;
typedef struct _CstILayerInterface CstILayerInterface;


struct _CstILayerInterface {
  SysTypeInterface parent;

  /* <private> */
  CstLayerNode* (*get_root) (CstLayer* self);
  void (*check) (CstLayer *self, CstLayout *layout);
  void (*set_root) (CstLayer *self, CstLayerNode *root);
  void (*append_node) (CstLayer* self, CstLayerNode *parent, CstLayerNode* node);
  CstLayerNode *(*new_node) (CstLayer* self);
  void (*iterate_node) (CstLayer* self, CstLayerNode *lnode, CstLayerNodeFunc func, SysPointer user_data);
};


SysType cst_i_layer_get_type(void);

CstLayerNode* cst_i_layer_get_root (CstLayer* self);
void cst_i_layer_check (CstLayer *self, CstLayout *layout);
void cst_i_layer_set_root (CstLayer *self, CstLayerNode *root);
void cst_i_layer_append_node (CstLayer* self, CstLayerNode *parent, CstLayerNode* node);
void cst_i_layer_new_node (CstLayer* self);
void cst_i_layer_iterate_node (CstLayer* self, CstLayerNode *lnode, CstLayerNodeFunc func, SysPointer user_data);

SYS_END_DECLS

#endif
