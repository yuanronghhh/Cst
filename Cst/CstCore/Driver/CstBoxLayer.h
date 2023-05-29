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

  CstBoxLayerPrivate *priv;
};

struct _CstBoxLayerClass {
  CstLayerClass parent;
};

SYS_API SysType cst_box_layer_get_type(void);
SYS_API CstLayer * cst_box_layer_new_I(void);

SYS_API void cst_box_layer_print_tree(CstBoxLayer *self);

SYS_API CstNode *cst_box_layer_get_root (CstBoxLayer * self);
SYS_API void cst_box_layer_set_root (CstBoxLayer *self, CstNode *root);
SYS_API void cst_box_layer_insert_after (CstBoxLayer *self, CstNode *parent, CstNode *last_child, CstNode *nnode);

SYS_END_DECLS

#endif

