#include <CstCore/Driver/Css/CstCssClosure.h>


SYS_DEFINE_TYPE(CstCssClosure, cst_css_closure, SYS_TYPE_OBJECT);


/* closure */
void cst_css_closure_calc(CstCssClosure* self, CstLayout *layout, CstRenderNode *render_node) {
  sys_return_if_fail(self != NULL);

  sys_return_if_fail(self->func != NULL);
  self->func(render_node, layout, self->data);
}

CstCssClosure* cst_css_closure_clone(CstCssClosure *c) {
  sys_return_val_if_fail(c != NULL, NULL);

  CstCssClosure *nc = cst_css_closure_new_I(c->data, c->func, c->free);

  return nc;
}

/* object api */
static void cst_css_closure_dispose(SysObject* o) {
  CstCssClosure *self = CST_CSS_CLOSURE(o);

  if (self->free) {
    if (self->data != NULL) {
      self->free(self->data);
    }
  }

  SYS_OBJECT_CLASS(cst_css_closure_parent_class)->dispose(o);
}

static void cst_css_closure_construct(CstCssClosure* self, SysPointer data, CstCssFunc calc, SysDestroyFunc free) {
  self->data = data;
  self->func = calc;
  self->free = free;
}

CstCssClosure *cst_css_closure_new(void) {
  return sys_object_new(CST_TYPE_CSS_CLOSURE, NULL);
}

CstCssClosure* cst_css_closure_new_I(SysPointer data, CstCssFunc calc, SysDestroyFunc free) {
  CstCssClosure *o = cst_css_closure_new();

  cst_css_closure_construct(o, data, calc, free);

  return o;
}

static void cst_css_closure_class_init(CstCssClosureClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_closure_dispose;
}

static void cst_css_closure_init(CstCssClosure *self) {
}
