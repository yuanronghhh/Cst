#include <CstCore/Driver/CstAlgorithm.h>

SYS_DEFINE_TYPE(CstAlgorithm, cst_algorithm, SYS_TYPE_OBJECT);


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
  CstAlgorithm *self = CST_ALGORITHM(o);

  SYS_OBJECT_CLASS(cst_algorithm_parent_class)->dispose(o);
}

static void cst_algorithm_class_init(CstAlgorithmClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_algorithm_dispose;
}

void cst_algorithm_init(CstAlgorithm* self) {
}

