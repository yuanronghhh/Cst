#include <CstCore/Driver/Flex/CstFlexAlgorithm.h>

SYS_DEFINE_TYPE(CstFlexAlgorithm, cst_flex_algorithm, CST_TYPE_ALGORITHM);

/* object api */
static void cst_flex_algorithm_construct(CstAlgorithm *self) {
}

CstAlgorithm* cst_flex_algorithm_new(void) {
  return sys_object_new(CST_TYPE_FLEX_ALGORITHM, NULL);
}

CstAlgorithm* cst_flex_algorithm_new_I(void) {
  CstAlgorithm *o = cst_flex_algorithm_new();

  cst_flex_algorithm_construct(o);

  return o;
}

static void cst_flex_algorithm_dispose(SysObject* o) {
  CstFlexAlgorithm *self = CST_FLEX_ALGORITHM(o);

  SYS_OBJECT_CLASS(cst_flex_algorithm_parent_class)->dispose(o);
}

static void cst_flex_algorithm_class_init(CstFlexAlgorithmClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_flex_algorithm_dispose;
}

void cst_flex_algorithm_init(CstFlexAlgorithm* self) {
}

