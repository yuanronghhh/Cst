#include <CstCore/Driver/CstPaintContext.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/Css/CstCss.h>

SYS_DEFINE_TYPE(CstPaintContext, cst_paint_context, SYS_TYPE_OBJECT);


CstPaintContext* cst_paint_context_new(void) {
  return sys_object_new(CST_TYPE_PAINT_CONTEXT, NULL);
}

void cst_paint_context_paint_self (CstPaintContext *self, CstPaintNode *item, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

}

void cst_paint_context_paint_children (CstPaintContext *self, CstPaintNode *item, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

}

SysBool cst_paint_context_need_paint(CstPaintContext *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->need_repaint;
}

void cst_paint_context_calc_css(CstPaintContext* self, CstPaintNode* paint_node, CstLayout* layout) {
  sys_return_if_fail(self != NULL);

  FRContext* cr = cst_layout_get_cr(layout);

  fr_context_save(cr);
  cst_css_render_groups(self, self->css_groups, cr, layout);
}

/* object api */
CstPaintContext *cst_paint_context_new_I(void) {
  CstPaintContext *o = cst_paint_context_new();

  return o;
}

static void cst_paint_context_init(CstPaintContext *self) {
}

static void cst_paint_context_dispose(SysObject* o) {
  sys_return_if_fail(o != NULL);

  SYS_OBJECT_CLASS(cst_paint_context_parent_class)->dispose(o);
}

static void cst_paint_context_class_init(CstPaintContextClass* cls) {
  SysObjectClass* ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_paint_context_dispose;
}
