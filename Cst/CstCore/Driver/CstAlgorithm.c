#include <CstCore/Driver/CstAlgorithm.h>
#include <CstCore/Driver/FlexLayout/CstFlexAlgorithm.h>

SYS_DEFINE_TYPE(CstAlgorithm, cst_algorithm, SYS_TYPE_OBJECT);

static CstAlgorithm* g_flex_algorithm = NULL;

CstAlgorithm* cst_algorithm_get_by_type(SysType tp) {
  sys_return_val_if_fail(tp != 0, NULL);

  if (tp == CST_TYPE_FLEX_ALGORITHM) {
    return g_flex_algorithm;
  }
  sys_abort_N("%s", "match not support layout algorithm type.");

  return NULL;
}

void cst_algorithm_measure(CstAlgorithm *self,
    CstRenderNode * rnode, 
   CstRenderContext* ctx) {
  sys_return_if_fail(self != NULL);

  CstAlgorithmClass *cls = CST_ALGORITHM_GET_CLASS(self);
  sys_return_if_fail(cls->measure != NULL);

  cls->measure(self, rnode, ctx);
}

void cst_algorithm_layout(CstAlgorithm *self,
    CstRenderNode * rnode, 
    CstRenderContext * ctx) {
  sys_return_if_fail(self != NULL);

  CstAlgorithmClass *cls = CST_ALGORITHM_GET_CLASS(self);
  sys_return_if_fail(cls->layout != NULL);

  cls->layout(self, rnode, ctx);
}

static void cst_algorithm_layout_i(CstAlgorithm* self,
    CstRenderNode * rnode, 
    CstRenderContext* ctx) {
}

void cst_algorithm_setup(void) {
  sys_assert(g_flex_algorithm == NULL && "algorithm must not init twice.");

  g_flex_algorithm = cst_flex_algorithm_new_I();
}

void cst_algorithm_teardown(void) {
  sys_assert(g_flex_algorithm != NULL && "algorithm must init before use");

  sys_clear_pointer(&g_flex_algorithm, _sys_object_unref);
}

/* object api */
static void cst_algorithm_construct(CstAlgorithm *self) {

}

CstAlgorithm* cst_algorithm_new(void) {
  return sys_object_new(CST_TYPE_ALGORITHM, NULL);
}

CstAlgorithm *cst_algorithm_new_I(void) {
  CstAlgorithm *o = cst_algorithm_new();

  cst_algorithm_construct(o);

  return o;
}

static void cst_algorithm_dispose(SysObject* o) {
  // CstAlgorithm *self = CST_ALGORITHM(o);

  SYS_OBJECT_CLASS(cst_algorithm_parent_class)->dispose(o);
}

static void cst_algorithm_class_init(CstAlgorithmClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_algorithm_dispose;
  cls->layout = cst_algorithm_layout_i;
}

void cst_algorithm_init(CstAlgorithm* self) {
}

