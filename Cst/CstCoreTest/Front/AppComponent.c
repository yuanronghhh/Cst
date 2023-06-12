/***
 * AppComponent auto generated from ComponentCode.code
 */

#include "AppComponent.h"

struct _AppComponentPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(AppComponent, app_component, CST_TYPE_COMPONENT);

/* user api */
AppComponent* app_component_new(void) {
  return sys_object_new(APP_TYPE_COMPONENT, NULL);
}

static void app_component_init(AppComponent *self) {
  self->priv = app_component_get_private(self);

}

static void app_component_class_init(AppComponentClass *cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = app_component_dispose;
}

static void app_component_dispose(SysObject *o) {
  SYS_OBJECT_CLASS(app_component_parent_class)->dispose(o);
}
