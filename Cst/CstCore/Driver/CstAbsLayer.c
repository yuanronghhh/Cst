#include <CstCore/Driver/CstAbsLayer.h>


struct _CstAbsLayerPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstAbsLayer, cst_abs_layer, CST_TYPE_LAYER);

/* object api */
CstLayer *cst_abs_layer_new(void) {
  return sys_object_new(CST_TYPE_ABS_LAYER, NULL);
}

static void cst_abs_layer_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_abs_layer_parent_class)->dispose(o);
}

void cst_abs_layer_construct(SysObject* o) {

  SYS_OBJECT_CLASS(cst_abs_layer_parent_class)->construct(o);
}

CstLayer * cst_abs_layer_new_I(void) {
  CstLayer * o = cst_abs_layer_new();

  cst_abs_layer_construct(SYS_OBJECT(o));

  return o;
}

static void cst_abs_layer_class_init(CstAbsLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)cst_abs_layer_construct;
  ocls->dispose = cst_abs_layer_dispose;
}

static void cst_abs_layer_init(CstAbsLayer *self) {
  self->priv = cst_abs_layer_get_private(self);
}
