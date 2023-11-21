/***
 * MiniMenuBase auto generated from ComponentCode.code
 */

#include <Mini/Front/MiniMenuBase.h>

SYS_DEFINE_TYPE(MiniMenuBase, mini_menubase, CST_TYPE_COMPONENT);

MiniMenuBase* mini_menubase_new(void) {
  return sys_object_new(MINI_TYPE_MENUBASE, NULL);
}

static void mini_menubase_init(MiniMenuBase *self) {
}

static void mini_menubase_construct(CstComponent *o, CstComponentContext *c) {

  CST_COMPONENT_CLASS(mini_menubase_parent_class)->construct(o, c);
}

static void mini_menubase_dispose(SysObject *o) {

  SYS_OBJECT_CLASS(mini_menubase_parent_class)->dispose(o);
}

static void mini_menubase_class_init(MiniMenuBaseClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstComponentClass *ncls = CST_COMPONENT_CLASS(cls);

  ocls->dispose = mini_menubase_dispose;
  ncls->construct = mini_menubase_construct;
}
