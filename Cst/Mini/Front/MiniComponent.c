/***
 * MiniComponent auto generated from ComponentCode.code
 */

#include <Mini/Front/MiniComponent.h>

struct _MiniComponentPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(MiniComponent, mini_component, CST_TYPE_COMPONENT);

/**
 * user functions
 */
FR_FUNC_DEFINE_EVENT(mini_quit_key) {
  FREventKey *ekey = FR_EVENT_KEY(e);
  CstApplication *app = cst_application_acquire();

  sys_debug_N("key press: %d,%d", fr_event_key_scancode(ekey), fr_event_key_action(ekey));

  cst_application_stop(app);

  return 0;
}

FR_FUNC_DEFINE_EVENT(mini_component_press2) {
  return 0;
}

MiniComponent* mini_component_new(void) {
  return sys_object_new(MINI_TYPE_COMPONENT, NULL);
}

static void mini_component_construct(CstComponent *comp, CstModule *v_module, CstComponent *v_parent) {
  CST_COMPONENT_CLASS(mini_component_parent_class)->construct(comp, v_module, v_parent);

  FRAWatchProps props = { 0 };
  props.key = FR_KEY_Q;

  cst_module_add_awatch(v_module, comp, "key", "mini_quit_key", mini_quit_key, &props);
  cst_module_set_function(v_module, FR_FUNC_ANY(mini_component_press2));
}

static void mini_component_init(MiniComponent *self) {
  self->priv = mini_component_get_private(self);
}

static void mini_component_dispose(SysObject *o) {

  SYS_OBJECT_CLASS(mini_component_parent_class)->dispose(o);
}

static void mini_component_class_init(MiniComponentClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstComponentClass *ccls = CST_COMPONENT_CLASS(cls);

  ocls->dispose = mini_component_dispose;
  ccls->construct = mini_component_construct;
}

