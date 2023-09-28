#include <CstCore/Front/CstBox.h>

SYS_DEFINE_TYPE(CstBox, cst_box, CST_TYPE_WIDGET);

CstBox* cst_box_new(void) {
  return sys_object_new(CST_TYPE_BOX);
}

static void cst_box_init(SysObject* o) {
  CstBox *self = CST_BOX(o);
  CstBoxPrivate* priv = o->priv = cst_box_get_private(self);

  cst_widget_set_name(CST_WIDGET(o), "Box");
}

static void cst_box_draw_i(CstWidget* self, FRCanvas* c) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(c != NULL);

  CstBoxPrivate* priv = self->priv;

  fr_canvas_set_source_rgba(c, 0.5, 0.3, 0.6, 1.0);
  fr_canvas_rectangle(c, 100, 100, 100, 100);
  fr_canvas_fill(c);
}

static void cst_box_class_init(CstBoxClass* cls) {
  SysObjectClass* ocls = (SysObjectClass*)cls;
  CstWidgetClass* wcls = (CstWidgetClass*)cls;

  ocls->dispose = cst_box_dispose;
  wcls->draw = cst_box_draw_i;
}

static void cst_box_dispose(SysObject* self) {
  CstBoxPrivate* priv = self->priv;

  SYS_OBJECT_CLASS(cst_box_parent_class)->dispose(self);
}
