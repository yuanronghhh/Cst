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

void fr_awatch_set_func(FRAWatch *self, FREventFunc func) {
  sys_return_if_fail(self != NULL);

  self->func = func;
}

FREventFunc fr_awatch_get_func(FRAWatch *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->func;
}

FRAWatch *fr_awatch_new(void) {
  return sys_object_new(FR_TYPE_AWATCH, NULL);
}

static SysObject *fr_awatch_dclone_i(SysObject *o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject *n = SYS_OBJECT_CLASS(fr_awatch_parent_class)->dclone(o);
  FRAWatch *oself = FR_AWATCH(o);
  FRAWatch *nself = FR_AWATCH(n);

  nself->func = oself->func;
  nself->func_name = sys_strdup(oself->func_name);
  nself->user_data = oself->user_data;
  nself->action_link = NULL;
  nself->action = oself->action;
  sys_object_ref(oself->action);

  // clone only for component now, this check maybe remove later.
  sys_assert(oself->user_data == NULL && "awatch bind user_data should be null when clone");
  sys_assert(oself->action_link == NULL && "awatch bind action_link should be null when clone");

  return n;
}

void fr_awatch_unbind(FRAWatch *self) {
  sys_return_if_fail(self != NULL);

  if(self->action_link == NULL) {
    sys_warning_N("action watch not bind, so no need to unbind: %s", self->func_name);
    return;
  }

  fr_action_unbind_awatch(self->action, self->action_link);
}

void fr_awatch_bind(FRAWatch *self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  if(self->action == NULL) {
    sys_abort_N("%s", "action must not be NULL");
  }

  self->action_link = fr_action_bind_awatch(self->action, self);
  self->user_data = user_data;
}

FRAWatch *fr_awatch_list_get_awatch(SysList *list, SysType atype, const SysChar *bind_var) {
  sys_return_val_if_fail(list != NULL, NULL);

  FRAWatch *w;

  sys_list_foreach(list, item) {
    w = item->data;

    if (w->func_name == NULL) { continue; }

    if (sys_object_is_a(w, atype) && sys_str_equal(bind_var, w->func_name)) {
      return w;
    }
  }

  return NULL;
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
    fr_awatch_unbind(self);
  }

  self->action_link = NULL;
  sys_clear_pointer(&self->action, _sys_object_unref);

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

static void fr_awatch_class_init(FRAWatchClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_awatch_dispose;
  ocls->dclone = fr_awatch_dclone_i;

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
