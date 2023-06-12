/***
 * AppButton auto generated from ComponentCode.code
 */

#include "AppButton.h"

struct _AppButtonPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(AppButton, app_button, CST_TYPE_COMPONENT);

FR_FUNC_DEFINE_EVENT(app_btn_clicked) {
  FREventKey *ekey = FR_EVENT_KEY(e);

  sys_debug_N("key release: %d,%d", fr_event_key_key(ekey), fr_event_key_scancode(ekey));

  return 0;
}

FR_FUNC_DEFINE_EVENT(app_btn_press) {
  FREventMouseKey *ekey = FR_EVENT_MOUSEKEY(e);

  sys_debug_N("key press: %d,%d", fr_event_mousekey_key(ekey), fr_event_mousekey_action(ekey));

  return 0;
}

AppButton* app_button_new(void) {
  return sys_object_new(APP_TYPE_BUTTON, NULL);
}

static void app_button_construct(CstComponent *self, CstModule *v_module, CstComponent *v_parent) {
  sys_return_if_fail(v_module != NULL);

  CST_COMPONENT_CLASS(app_button_parent_class)->construct(self, v_module, v_parent);

  cst_component_set_function(self, FR_FUNC_EVENT(app_btn_clicked));
  cst_component_set_function(self, FR_FUNC_EVENT(app_btn_press));
}

static void app_button_init(AppButton *self) {
  self->priv = app_button_get_private(self);
}

static void app_button_class_init(AppButtonClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstComponentClass *ccls = CST_COMPONENT_CLASS(cls);

  ccls->construct = app_button_construct;
  ocls->dispose = app_button_dispose;
}

static void app_button_dispose(SysObject *o) {
  SYS_OBJECT_CLASS(app_button_parent_class)->dispose(o);
}
