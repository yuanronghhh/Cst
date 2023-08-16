#include <CstCore/Front/Common/CstLayoutContext.h>


struct _CstLayoutContextPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(CstLayoutContext, cst_layout_context, SYS_TYPE_OBJECT);

CstLayoutContext* cst_layout_context_new(void) {
  return sys_object_new(CST_TYPE_LAYOUT_CONTEXT, NULL);
}

CstLayoutContext *cst_layout_context_new_I(SysInt x, SysInt y) {
  CstLayoutContext *o = cst_layout_context_new();

  o->x = x;
  o->y = y;
  o->width = 0;
  o->height = 0;

  return o;
}

/* object api */
static void cst_layout_context_init(CstLayoutContext *self) {
  self->priv = cst_layout_context_get_private(self);
}

static void cst_layout_context_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_layout_context_parent_class)->dispose(o);
}

static void cst_layout_context_class_init(CstLayoutContextClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_layout_context_dispose;
}

