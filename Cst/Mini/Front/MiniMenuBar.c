/***
 * MiniMenuBar auto generated from ComponentCode.code
 */

#include <Mini/Front/MiniMenuBar.h>
#include <Mini/Front/MiniComponent.h>


struct _MiniMenuBarPrivate {
  SysInt reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(MiniMenuBar, mini_menubar, CST_TYPE_COMPONENT);


FR_FUNC_DEFINE_EVENT(menubar_btn_press) {
  FREventMouseKey *ekey = FR_EVENT_MOUSEKEY(e);

  mini_component_press2(e, user_data);

  sys_debug_N("key press: %d,%d", fr_event_mousekey_key(ekey), fr_event_mousekey_action(ekey));

  return 0;
}

MiniMenuBar* mini_menubar_new(void) {
  return sys_object_new(MINI_TYPE_MENUBAR, NULL);
}

static void mini_menubar_init(MiniMenuBar *self) {
  self->priv = mini_menubar_get_private(self);
}

static void mini_menubar_construct(CstComponent *comp, CstModule *v_module, CstComponent *v_parent) {
  CST_COMPONENT_CLASS(mini_menubar_parent_class)->construct(comp, v_module, v_parent);

  cst_component_set_function(comp, FR_FUNC_EVENT(menubar_btn_press));
}

static void mini_menubar_dispose(SysObject *o) {

  SYS_OBJECT_CLASS(mini_menubar_parent_class)->dispose(o);
}

static void mini_menubar_class_init(MiniMenuBarClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstComponentClass *ccls = CST_COMPONENT_CLASS(cls);

  ocls->dispose = mini_menubar_dispose;
  ccls->construct = mini_menubar_construct;
}
