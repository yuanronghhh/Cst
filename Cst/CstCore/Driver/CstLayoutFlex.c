#include <CstCore/Driver/CstLayoutFlex.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLayoutFlex, cst_layout_flex, SYS_TYPE_OBJECT);


void cst_layout_flex_layout(CstLayoutFlex* self, CstBoxNode *box_node) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(render_node != NULL);

  CstBoxNode *node;

  for(node = box_node->children; node; node = node->next) {

    cst_render_context_layout(node);
  }
}

/* object api */
CstLayoutFlex* cst_layout_flex_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT, NULL);
}

static void cst_layout_flex_init(CstLayoutFlex *self) {
}

static void cst_layout_flex_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_flex_parent_class)->dispose(o);
}

static void cst_layout_flex_class_init(CstLayoutFlexClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_flex_dispose;
}
