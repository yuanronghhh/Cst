#include <CstCore/Driver/CstBuilder.h>


SYS_DEFINE_TYPE(CstBuilder, cst_builder, SYS_TYPE_OBJECT);


void cst_builder_parse(CstBuilder *self, CstParser *ps) {
  CstBuilderClass *cls = CST_BUILDER_GET_CLASS(self);

  sys_return_if_fail(cls->parse != NULL);

  cls->parse(self, ps);
}

void cst_builder_build(CstBuilder *self, SysObject *o) {
  CstBuilderClass *cls = CST_BUILDER_GET_CLASS(self);

  sys_return_if_fail(cls->build != NULL);

  cls->build(self, o);
}

/* object api */
static void cst_builder_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(cst_builder_parent_class)->dispose(o);
}

CstBuilder *cst_builder_new(void) {
  return sys_object_new(CST_TYPE_BUILDER, NULL);
}

static void cst_builder_class_init(CstBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_builder_dispose;
  cls->parse = cst_builder_parse_i;
  cls->build = cst_builder_build_i;
}

static void cst_builder_init(CstBuilder *self) {
}
