#include <CstCore/Driver/CstLayer.h>

struct _CstLayerPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLayer, cst_layer, SYS_TYPE_OBJECT);

void cst_layer_check(CstLayer *self, CstRender *v_render, FRRect *bound) {
  sys_return_if_fail(self != NULL);

  CstLayerClass *cls = CST_LAYER_GET_CLASS(self);

  cls->check(self, v_render, bound);
}

void cst_layer_render(CstLayer *self, CstModule *v_module, CstRender *v_render) {
  sys_return_if_fail(self != NULL);

  CstLayerClass *cls = CST_LAYER_GET_CLASS(self);

  if (cls->render) {
    cls->render(self, v_module, v_render);
  }
}

/* object api */
static void cst_layer_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_layer_parent_class)->dispose(o);
}

void cst_layer_construct(SysObject* o) {

  SYS_OBJECT_CLASS(cst_layer_parent_class)->construct(o);
}

CstLayer *cst_layer_new(void) {
  return sys_object_new(CST_TYPE_LAYER, NULL);
}

static void cst_layer_class_init(CstLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)cst_layer_construct;
  ocls->dispose = cst_layer_dispose;
}

static void cst_layer_init(CstLayer *self) {
  self->priv = cst_layer_get_private(self);
}
