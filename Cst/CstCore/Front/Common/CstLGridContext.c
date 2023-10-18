#include <CstCore/Front/Common/CstLGridContext.h>
#include <CstCore/Driver/CstRenderNode.h>


SYS_DEFINE_TYPE(CstLGridContext, cst_lgrid_context, CST_TYPE_LAYOUT_CONTEXT);

/* sys object api */
CstRenderContext* cst_lgrid_context_new(void) {
  return sys_object_new(CST_TYPE_LGRID_CONTEXT, NULL);
}

static void cst_lgrid_context_init(CstLGridContext *o) {
}

static void cst_lgrid_context_dispose(SysObject* o) {
  SYS_OBJECT_CLASS(cst_lgrid_context_parent_class)->dispose(o);
}

static void cst_lgrid_context_class_init(CstLGridContextClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstRenderContextClass *ncls = CST_RENDER_CONTEXT_CLASS(cls);

  ocls->dispose = cst_lgrid_context_dispose;
}
