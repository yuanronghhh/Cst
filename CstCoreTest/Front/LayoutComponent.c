/***
 * LayoutComponent auto generated from ComponentCode.code
 */

#include <CstCoreTest/Front/LayoutComponent.h>

struct _LayoutComponentPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(LayoutComponent, layout_component, CST_TYPE_COMPONENT);

LayoutComponent* layout_component_new(void) {
  return sys_object_new(LAYOUT_TYPE_COMPONENT, NULL);
}

static void layout_component_init(LayoutComponent *self) {
  self->priv = layout_component_get_private(self);
}

static void layout_component_class_init(LayoutComponentClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = layout_component_dispose;
}

static void layout_component_dispose(SysObject *o) {

}
