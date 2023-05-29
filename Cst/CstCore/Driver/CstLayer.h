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

  CstLayerPrivate *priv;
};

struct _CstLayerClass {
  SysObjectClass parent;

  void (*check) (CstLayer *layer, CstRender *v_render, FRRect *bound);
  void (*render) (CstLayer *layer, CstModule *v_module, CstRender *v_render);
};

SYS_API SysType cst_layer_get_type(void);
SYS_API CstLayer *cst_layer_new(void);

SYS_API void cst_layer_check(CstLayer * self, CstRender *v_render, FRRect * bound);
SYS_API void cst_layer_render(CstLayer *layer, CstModule *v_module, CstRender *v_render);

SYS_END_DECLS

#endif

