#include <CstCore/Driver/CstAbsLayer.h>


SYS_DEFINE_TYPE(CstAbsLayer, cst_abs_layer, CST_TYPE_LAYER);

void cst_abs_layer_check_i(CstLayer *self, FRDraw *draw, FRRegion *region) {
}

/* object api */
CstLayer *cst_abs_layer_new(void) {
  return sys_object_new(CST_TYPE_ABS_LAYER, NULL);
}

static void cst_abs_layer_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_abs_layer_parent_class)->dispose(o);
}

void cst_abs_layer_construct(CstLayer* o) {

  CST_LAYER_CLASS(cst_abs_layer_parent_class)->construct(o);
}

CstLayer * cst_abs_layer_new_I(void) {
  CstLayer * o = cst_abs_layer_new();

  cst_abs_layer_construct(o);

  return o;
}

static void cst_abs_layer_class_init(CstAbsLayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstLayerClass *lcls = CST_LAYER_CLASS(cls);

  lcls->construct = cst_abs_layer_construct;
  ocls->dispose = cst_abs_layer_dispose;
}

static void cst_abs_layer_init(CstAbsLayer *self) {
}
