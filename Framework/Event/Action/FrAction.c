#include <Framework/Event/Action/FrAction.h>
#include <Framework/Event/Action/FrAWatch.h>
#include <Framework/Event/FrEvents.h>


SYS_DEFINE_TYPE(FrAction, fr_action, SYS_TYPE_OBJECT);

SysBool fr_action_check(FrAction *self, FrEvent *e) {
  FrActionClass *cls = FR_ACTION_GET_CLASS(self);

  return cls->check(self, e);
}

void fr_action_dispatch(FrAction *self, FrEvent *e) {
  FrActionClass *cls = FR_ACTION_GET_CLASS(self);

  cls->dispatch(self, e);
}

SysBool fr_action_check_i(FrAction *self, FrEvent *e) {
  return true;
}

void fr_action_unbind_awatch(FrAction *self, SysList *action_link) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(action_link != NULL);

  self->awatch_list = sys_list_delete_link(self->awatch_list, action_link);
}

SysList* fr_action_bind_awatch(FrAction *self, FrAWatch *awatch) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(awatch != NULL, NULL);

  self->awatch_list = sys_list_prepend(self->awatch_list, awatch);

  return self->awatch_list;
}

static void fr_action_dispatch_i(FrAction *self, FrEvent *e) {
  sys_return_if_fail(self != NULL);

  FrAWatch *awatch;

  if(self->awatch_list == NULL) {
    return;
  }

  for(SysList *item = self->awatch_list; item; item = item->next) {
    awatch = item->data;

    if(fr_awatch_check(awatch, e)) {
      fr_awatch_dispatch(awatch, e);
    }
  }
}

const SysChar *fr_action_get_name(FrAction *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

void fr_action_create(FrAction *self, FrActionContext *info) {
  FrActionClass *cls = FR_ACTION_GET_CLASS(self);

  sys_return_if_fail(cls->create != NULL);

  cls->create(self, info);
}

/* object api */
void fr_action_create_i(FrAction *self, FrActionContext *info) {
  self->awatch_list = NULL;
  self->name = sys_strdup(info->name);
}

FrAction* fr_action_new(void) {
  return sys_object_new(FR_TYPE_ACTION, NULL);
}

FrAction *fr_action_new_I(FrActionContext *info) {
  sys_return_val_if_fail(info != NULL, NULL);
  sys_return_val_if_fail(info->name != NULL, NULL);

  FrAction *o = fr_action_new();

  fr_action_create_i(o, info);

  return o;
}

static void fr_action_dispose(SysObject* o) {
  FrAction *self = FR_ACTION(o);

  sys_list_free_full(self->awatch_list, (SysDestroyFunc)_sys_object_unref);
  sys_clear_pointer(&self->name, sys_free);

  SYS_OBJECT_CLASS(fr_action_parent_class)->dispose(o);
}

static void fr_action_class_init(FrActionClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->create = fr_action_create_i;
  cls->check = fr_action_check_i;
  cls->dispatch = fr_action_dispatch_i;

  ocls->dispose = fr_action_dispose;
}

void fr_action_init(FrAction *self) {
}

FrAction* fr_action_get_static(void) {
  static FrAction *node = NULL;

  fr_events_lock();
  if(node != NULL) {
    goto done;
  }
  FrActionContext info = {.name = "action"};
  node = fr_action_new_I(&info);

done:
  fr_events_unlock();
  return node;
}
