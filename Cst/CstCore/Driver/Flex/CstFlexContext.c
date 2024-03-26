#include <CstCore/Driver/Flex/CstFlexContext.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/Flex/CstFlexItem.h>

SYS_DEFINE_TYPE(CstFlexContext, cst_flex_context, SYS_TYPE_OBJECT);

void cst_flex_context_add_line(CstFlexContext *self, CstFlexLine *line) {
  sys_harray_add(&self->lines, line);
}

SysHArray * cst_flex_context_get_lines(CstFlexContext *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return &self->lines;
}

void cst_flex_context_add_nodes(CstFlexContext *self, CstRenderNode* rnode) {
  //CstBoxNode *cnode = cst_render_node_children(rnode);
  //if (cnode == NULL) { return; }

  //for(; cnode; cnode = cst_box_node_next(cnode)) {
  //  CstFlexItem *item = CST_FLEX_ITEM(cst_render_node(cnode));

  //  sys_harray_add(&self->lines, item);
  //}
}

/* object api */
static void cst_flex_context_construct(CstFlexContext *self) {
}

CstFlexContext* cst_flex_context_new(void) {
  return sys_object_new(CST_TYPE_FLEX_CONTEXT, NULL);
}

CstFlexContext *cst_flex_context_new_I() {
  CstFlexContext *o = cst_flex_context_new();

  cst_flex_context_construct(o);

  return o;
}

static void cst_flex_context_dispose(SysObject* o) {
  CstFlexContext *self = CST_FLEX_CONTEXT(o);

  SYS_OBJECT_CLASS(cst_flex_context_parent_class)->dispose(o);
}

static void cst_flex_context_class_init(CstFlexContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_flex_context_dispose;
}

void cst_flex_context_init(CstFlexContext* self) {
  sys_harray_init_with_free_func(&self->lines, (SysDestroyFunc)_sys_object_unref);
}

