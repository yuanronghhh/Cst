/***
 * AppBox auto generated from ComponentCode.code
 */

#include "AppBox.h"

SYS_DEFINE_TYPE(AppBox, app_box, CST_TYPE_COMPONENT);

AppBox* app_box_new(void) {
  return sys_object_new(APP_TYPE_BOX, NULL);
}

static void app_box_construct(CstComponent *o, CstComponentContext *c) {

  CST_COMPONENT_CLASS(app_box_parent_class)->construct(o, c);
}

static void app_box_init(AppBox *self) {
}

static void app_box_class_init(AppBoxClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstComponentClass *ccls = CST_COMPONENT_CLASS(cls);

  ccls->construct = app_box_construct;
  ocls->dispose = app_box_dispose;
}

static void app_box_dispose(SysObject *o) {

  SYS_OBJECT_CLASS(app_box_parent_class)->dispose(o);
}
