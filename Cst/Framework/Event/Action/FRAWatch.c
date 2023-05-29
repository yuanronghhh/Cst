#include <Framework/Event/Action/FRAWatch.h>
#include <Framework/Event/Action/FRAction.h>
#include <Framework/Event/Action/FRAWatchMousePress.h>
#include <Framework/Event/Action/FRAWatchKey.h>
#include <Framework/Event/Action/FRAWatchKeyPress.h>


struct _FRAWatchPrivate {
  FRAction *action;
  FREventFunc func;
  SysPointer user_data;
  SysList *action_link;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAWatch, fr_awatch, SYS_TYPE_OBJECT);

static SysHashTable *g_awatch_nodes = NULL;


SysPointer fr_awatch_get_data(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatchPrivate *priv = self->priv;

  return priv->user_data;
}

FRAction *fr_awatch_get_action(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatchPrivate *priv = self->priv;

  return priv->action;
}

void fr_awatch_set_action(FRAWatch *self, FRAction *action) {
  sys_return_if_fail(self != NULL);

  FRAWatchPrivate *priv = self->priv;

  priv->action = action;
}

void fr_action_watch_register_type(const SysChar *name, SysType type) {
  sys_hash_table_insert(g_awatch_nodes, (SysPointer)sys_strdup(name), (SysPointer)type);
}

static SysType fr_awatch_get_by_name(const SysChar *name) {
  return (SysType)sys_hash_table_lookup(g_awatch_nodes, (SysPointer)name);
}

void fr_awatch_dispatch(FRAWatch *self, FREvent *e) {
  sys_return_if_fail(self != NULL);

  FRAWatchClass *cls = FR_AWATCH_GET_CLASS(self);

  cls->dispatch(self, e);
}

SysBool fr_awatch_check(FRAWatch *self, FREvent *e) {
  sys_return_val_if_fail(self != NULL, false);

  FRAWatchClass *cls = FR_AWATCH_GET_CLASS(self);

  return cls->check(self, e);
}

static SysBool fr_awatch_check_i(FRAWatch *self, FREvent *e) {
  return false;
}

static void fr_awatch_dispatch_i(FRAWatch *self, FREvent *e) {
  sys_return_if_fail(self != NULL);

  FRAWatchPrivate *priv = self->priv;

  priv->func(e, priv->user_data);
}

FRAWatch *fr_awatch_new(void) {
  return sys_object_new(FR_TYPE_AWATCH, NULL);
}

FRAWatch *fr_awatch_clone(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatchClass *cls = FR_AWATCH_GET_CLASS(self);

  return cls->dclone(self);
}

static FRAWatch *fr_awatch_dclone_i(FRAWatch *old) {
  sys_return_val_if_fail(old != NULL, NULL);
  SysType type = sys_type_from_instance(old);

  FRAWatch *nawatch = sys_object_new(type, NULL);

  FRAWatchPrivate *opriv = old->priv;
  FRAWatchPrivate *npriv = nawatch->priv;

  npriv->func = opriv->func;
  npriv->user_data = opriv->user_data;
  npriv->action_link = NULL;
  npriv->action = fr_action_ref(opriv->action);

  // clone only for component now, this check maybe remove later.
  sys_assert(opriv->user_data == NULL && "awatch bind user_data should be null when clone");
  sys_assert(opriv->action_link == NULL && "awatch bind action_link should be null when clone");

  return nawatch;
}

void fr_awatch_bind(FRAWatch *self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  FRAWatchPrivate *priv = self->priv;

  if(priv->action == NULL) {
    sys_abort_N("%s", "action must not be NULL");
  }

  priv->action_link = fr_action_bind_awatch(priv->action, self);
  priv->user_data = user_data;
}

void fr_awatch_create(FRAWatch* self, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  sys_return_if_fail(self != NULL);

  FRAWatchClass *cls = FR_AWATCH_GET_CLASS(self);

  cls->create(self, func_name, func, props);
}

/* object api */
static void fr_awatch_dispose(SysObject* o) {
  FRAWatch *self = FR_AWATCH(o);
  FRAWatchPrivate *priv = self->priv;

  if (priv->action_link) {
    fr_action_unbind_awatch(priv->action, priv->action_link);
  }

  priv->action_link = NULL;
  sys_object_unref(priv->action);

  SYS_OBJECT_CLASS(fr_awatch_parent_class)->dispose(o);
}

static void fr_awatch_create_i(FRAWatch* self, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FRAWatchPrivate* priv = self->priv;

  priv->user_data = NULL;
  priv->func = func;
  sys_object_ref(priv->action);
}

FRAWatch *fr_awatch_new_by_name(const SysChar *watch_name, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  sys_return_val_if_fail(props != NULL, NULL);

  SysType type = fr_awatch_get_by_name(watch_name);
  FRAWatch *o = sys_object_new(type, NULL);

  fr_awatch_create(o, func_name, func, props);

  return o;
}

FRAWatch *fr_awatch_new_bind(SysPointer user_data, const SysChar *watch_name, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  sys_return_val_if_fail(watch_name != NULL, NULL);
  sys_return_val_if_fail(func_name != NULL, NULL);
  sys_return_val_if_fail(func != NULL, NULL);
  sys_return_val_if_fail(props != NULL, NULL);

  FRAWatch *awatch = fr_awatch_new_by_name(watch_name, func_name, func, props);
  fr_awatch_bind(awatch, user_data);

  return awatch;
}

static void fr_awatch_class_init(FRAWatchClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_awatch_dispose;

  cls->dclone = fr_awatch_dclone_i;
  cls->create = fr_awatch_create_i;
  cls->check = fr_awatch_check_i;
  cls->dispatch = fr_awatch_dispatch_i;
}

static void fr_awatch_init(FRAWatch *self) {
  self->priv = fr_awatch_get_private(self);
}

void fr_awatch_setup(void) {
  g_awatch_nodes = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);

  fr_action_watch_register_type("mouse_press", FR_TYPE_AWATCH_MOUSE_PRESS);
  fr_action_watch_register_type("key", FR_TYPE_AWATCH_KEY);
  fr_action_watch_register_type("key_press", FR_TYPE_AWATCH_KEY_PRESS);
}

void fr_awatch_teardown(void) {
  sys_hash_table_unref(g_awatch_nodes);
}
