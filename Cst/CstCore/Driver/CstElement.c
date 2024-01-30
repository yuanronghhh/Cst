#include <CstCore/Driver/CstElement.h>

SYS_DEFINE_TYPE(CstElement, cst_element, SYS_TYPE_OBJECT);


/* object api */
static void cst_element_construct(CstElement *self) {

}

CstElement* cst_element_new(void) {
  return sys_object_new(CST_ELEMENT_TYPE, NULL);
}

CstElement *cst_element_new_I(void) {
  CstElement *o = cst_element_new();

  cst_element_construct(o);

  return o;
}

static void cst_element_dispose(SysObject* o) {
  CstElement *self = CST_ELEMENT(o);

  SYS_OBJECT_CLASS(cst_element_parent_class)->dispose(o);
}

static void cst_element_class_init(CstElementClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_element_dispose;
}

void cst_element_init(CstElement* self) {
}

