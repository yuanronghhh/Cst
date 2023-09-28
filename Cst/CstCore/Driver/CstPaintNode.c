#include <CstCore/Driver/CstPaintNode.h>

SYS_DEFINE_TYPE(CstPaintNode, cst_paint_node, SYS_TYPE_OBJECT);

/* object api */
CstPaintNode* cst_paint_node_new(void) {
  return sys_object_new(CST_TYPE_PAINT_NODE, NULL);
}

static void cst_paint_node_construct(CstPaintNode *self) {
}

SysBool cst_paint_node_is_visible(CstPaintNode *self) {
  sys_return_val_if_fail(self != NULL, false);
  
  return self->is_visible;
}

CstPaintNode *cst_paint_node_new_I(void) {
  CstPaintNode * o = cst_paint_node_new();

  cst_paint_node_construct(o);

  return o;
}

static void cst_paint_node_init(CstPaintNode *self) {
}

static void cst_paint_node_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_paint_node_parent_class)->dispose(o);
}

static void cst_paint_node_class_init(CstPaintNodeClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_paint_node_dispose;
}
