#include <Framework/Event/Action/FRAWatch.h>
#include <Framework/Event/Action/FRAction.h>

// watch list
#include <Framework/Event/Action/FRAWatchMousePress.h>
#include <Framework/Event/Action/FRAWatchMouseRelease.h>
#include <Framework/Event/Action/FRAWatchKey.h>
#include <Framework/Event/Action/FRAWatchKeyPress.h>
#include <Framework/Event/Action/FRAWatchAny.h>
#include <Framework/Event/Action/FRAWatchRefresh.h>


SYS_DEFINE_TYPE(FRAWatch, fr_awatch, SYS_TYPE_OBJECT);

static SysHashTable *g_awatch_nodes = NULL;

SysPointer fr_awatch_get_data(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->user_data;
}

FRAction *fr_awatch_get_action(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->action;
}

void fr_awatch_set_action(FRAWatch *self, FRAction *action) {
  sys_return_if_fail(self != NULL);

  self->action = action;
}

void fr_action_watch_register_type(const SysChar *name, SysType type) {
  sys_hash_table_insert(g_awatch_nodes, (SysPointer)sys_strdup(name), (SysPointer)type);
}

SysType fr_awatch_get_type_by_name(const SysChar *name) {
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

  sys_return_if_fail(self->func != NULL && "awatch bind is correct ?");

  self->func(e, self->user_data);
}

const SysChar *fr_awatch_get_func_name(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->func_name;
}

void fr_awatch_set_func_name(FRAWatch *self, const SysChar *func_name) {
  sys_return_if_fail(self != NULL);

  if (self->func_name) {
    sys_clear_pointer(&self->func_name, sys_free);
  }

  self->func_name = sys_strdup(func_name);
}

void fr_awatch_set_function(FRAWatch *self, FREventFunc func) {
  sys_return_if_fail(self != NULL);

  self->func = func;
}

FRAWatch *fr_awatch_new(void) {
  return sys_object_new(FR_TYPE_AWATCH, NULL);
}

FRAWatch *fr_awatch_clone(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  FRAWatchClass *cls = FR_AWATCH_GET_CLASS(self);

  return cls->dclone(self);
}

static FRAWatch *fr_awatch_dclone_i(FRAWatch *oself) {
  sys_return_val_if_fail(oself != NULL, NULL);
  SysType type = sys_type_from_instance(oself);

  FRAWatch *nself = sys_object_new(type, NULL);

  nself->func = oself->func;
  nself->func_name = sys_strdup(oself->func_name);
  nself->user_data = oself->user_data;
  nself->action_link = NULL;
  nself->action = fr_action_ref(oself->action);

  // clone only for component now, this check maybe remove later.
  sys_assert(oself->user_data == NULL && "awatch bind user_data should be null when clone");
  sys_assert(oself->action_link == NULL && "awatch bind action_link should be null when clone");

  return nself;
}

void fr_awatch_bind(FRAWatch *self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  if(self->action == NULL) {
    sys_abort_N("%s", "action must not be NULL");
  }

  self->action_link = fr_action_bind_awatch(self->action, self);
  self->user_data = user_data;
}

void fr_awatch_create(FRAWatch* self, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  sys_return_if_fail(self != NULL);

  FRAWatchClass *cls = FR_AWATCH_GET_CLASS(self);

  cls->create(self, func_name, func, props);
}

/* object api */
static void fr_awatch_dispose(SysObject* o) {
  FRAWatch *self = FR_AWATCH(o);
  if (self->action_link) {
    fr_action_unbind_awatch(self->action, self->action_link);
  }

  self->action_link = NULL;
  sys_object_unref(self->action);

  if (self->func_name) {
    sys_clear_pointer(&self->func_name, sys_free);
  }

  SYS_OBJECT_CLASS(fr_awatch_parent_class)->dispose(o);
}

static void fr_awatch_create_i(FRAWatch* self, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  self->user_data = NULL;
  self->func = func;
  self->func_name = func_name != NULL ? sys_strdup(func_name) : NULL;

  sys_object_ref(self->action);
}

FRAWatch *fr_awatch_new_by_name(const SysChar *watch_name, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  sys_return_val_if_fail(props != NULL, NULL);

  SysType type = fr_awatch_get_type_by_name(watch_name);
  if (type == 0) {
    return NULL;
  }

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
  if (awatch == NULL) {
    sys_warning_N("Not found watch: %s,%s", watch_name, func_name);
    return NULL;
  }

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
}

void fr_awatch_setup(void) {
  g_awatch_nodes = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, sys_free, NULL);

  fr_action_watch_register_type("mouse_release", FR_TYPE_AWATCH_MOUSE_RELEASE);
  fr_action_watch_register_type("mouse_press", FR_TYPE_AWATCH_MOUSE_PRESS);
  fr_action_watch_register_type("key", FR_TYPE_AWATCH_KEY);
  fr_action_watch_register_type("key_press", FR_TYPE_AWATCH_KEY_PRESS);
  fr_action_watch_register_type("window_resize", FR_TYPE_AWATCH_ANY);
  fr_action_watch_register_type("window_refresh", FR_TYPE_AWATCH_REFRESH);
}

void fr_awatch_teardown(void) {
  sys_hash_table_unref(g_awatch_nodes);
}
