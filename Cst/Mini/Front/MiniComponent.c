/***
 * MiniComponent auto generated from ComponentCode.code
 */

#include <Mini/Front/MiniComponent.h>

SYS_DEFINE_TYPE(MiniComponent, mini_component, CST_TYPE_COMPONENT);

/**
 * user functions
 */
FR_FUNC_DEFINE_EVENT(mini_quit_key) {
  CstApplication *app = cst_application_acquire();

  cst_application_stop(app);

  return 0;
}

FR_FUNC_DEFINE_EVENT(mini_menu_press) {
  CstNode *node = CST_NODE(user_data);

  sys_debug_N("%s", cst_node_get_id(node));

  return 0;
}

MiniComponent* mini_component_new(void) {
  return sys_object_new(MINI_TYPE_COMPONENT, NULL);
}

static void mini_component_construct(CstComponent *o, CstComponentBuilder *builder) {
  CST_COMPONENT_CLASS(mini_component_parent_class)->construct(o, builder);
  CstModule *v_module;
  FRAWatch *awatch;

  v_module = cst_component_builder_get_v_module(builder);

  awatch = fr_awatch_key_new_I(FR_KEY_Q, "mini_quit_key", mini_quit_key);
  fr_awatch_bind(awatch, (SysPointer)o);

  cst_module_add_awatch(v_module, awatch);
  cst_module_set_function(v_module, FR_FUNC_EVENT(mini_menu_press));
  cst_module_set_function(v_module, FR_FUNC_EVENT(mini_quit_key));
}

static void mini_component_init(MiniComponent *self) {
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

