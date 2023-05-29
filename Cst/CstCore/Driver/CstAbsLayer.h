
#ifndef __CST_ABS_LAYER_H__
#define __CST_ABS_LAYER_H__

#include <CstCore/Driver/CstLayer.h>

SYS_BEGIN_DECLS

#define CST_TYPE_ABS_LAYER (cst_abs_layer_get_type())
#define CST_ABS_LAYER(o) ((CstAbsLayer* )sys_object_cast_check(o, CST_TYPE_ABS_LAYER))
#define CST_ABS_LAYER_CLASS(o) ((CstAbsLayerClass *)sys_class_cast_check(o, CST_TYPE_ABS_LAYER))
#define CST_ABS_LAYER_GET_CLASS(o) sys_instance_get_class(o, CstAbsLayerClass)


struct _CstAbsLayer {
  SysObject parent;

  CstAbsLayerPrivate *priv;
};

struct _CstAbsLayerClass {
  SysObjectClass parent;
};

SYS_API SysType cst_abs_layer_get_type(void);
SYS_API CstLayer *cst_abs_layer_new_I(void);

SYS_END_DECLS

#endif
