#ifndef __CST_BOX_LAYER_H__
#define __CST_BOX_LAYER_H__

#include <CstCore/Driver/CstLayer.h>

SYS_BEGIN_DECLS


#define CST_TYPE_BOX_LAYER (cst_box_layer_get_type())
#define CST_BOX_LAYER(o) ((CstBoxLayer* )sys_object_cast_check(o, CST_TYPE_BOX_LAYER))
#define CST_BOX_LAYER_CLASS(o) ((CstBoxLayerClass *)sys_class_cast_check(o, CST_TYPE_BOX_LAYER))
#define CST_BOX_LAYER_GET_CLASS(o) sys_instance_get_class(o, CstBoxLayerClass)


struct _CstBoxLayer {
  CstLayer parent;

  /* <private> */
  CstBoxNode *tree;
  SysList *gap_nodes;
};

struct _CstBoxLayerClass {
  CstLayerClass parent;
};

SysType cst_box_layer_get_type(void);
CstLayer* cst_box_layer_new(void);
CstLayer * cst_box_layer_new_I(void);

void cst_box_layer_set_root (CstBoxLayer *self, CstBoxNode *root);

void cst_box_layer_print_tree(CstBoxLayer* self);

SYS_END_DECLS

#endif

