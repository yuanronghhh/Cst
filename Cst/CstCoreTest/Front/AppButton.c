/***
 * AppButton auto generated from ComponentCode.code
 */

#include "AppButton.h"

SYS_DEFINE_TYPE(AppButton, app_button, CST_TYPE_COMPONENT);

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

static void app_button_construct(CstComponent *self, CstComponentContext *builder) {
  sys_return_if_fail(builder != NULL);

  CstModule *v_module = cst_component_context_get_v_module(builder);

  CST_COMPONENT_CLASS(app_button_parent_class)->construct(self, builder);

  cst_module_set_function(v_module, FR_FUNC_EVENT(app_btn_clicked));
  cst_module_set_function(v_module, FR_FUNC_EVENT(app_btn_press));
}

static void app_button_init(AppButton *self) {
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


