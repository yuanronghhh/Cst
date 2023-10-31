#include <CstCore/Driver/CstBuilder.h>


SYS_DEFINE_TYPE(CstBuilder, cst_builder, SYS_TYPE_OBJECT);


void cst_builder_parse_i(CstBuilder *self, CstParser *ps) {
  sys_return_if_fail(self != NULL);
}

void cst_builder_build_i(CstBuilder *self, SysObject *o) {
  sys_return_if_fail(self != NULL);
}

/* object api */
static void cst_builder_dispose(SysObject* o) {
  CstBuilder *self = CST_BUILDER(o);

  if(self->parser) {
    sys_clear_pointer(&self->parser, _sys_object_unref);
  }

  SYS_OBJECT_CLASS(cst_builder_parent_class)->dispose(o);
}

CstBuilder *cst_builder_new(void) {
  return sys_object_new(CST_TYPE_BUILDER, NULL);
}

static void cst_builder_construct(CstBuilder *self, CstParser *parser) {
  self->parser = parser;
}

CstBuilder *cst_builder_new_I(CstParser *parser) {
  CstBuilder *o = cst_builder_new();

  cst_builder_construct(o, parser);

  return o;
}

static void cst_builder_class_init(CstBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_builder_dispose;
  cls->construct = cst_builder_construct;
  cls->parse = cst_builder_parse_i;
  cls->build = cst_builder_build_i;
}

static void cst_builder_init(CstBuilder *self) {
}
