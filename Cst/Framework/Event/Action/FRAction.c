#include <Framework/Event/Action/FRAction.h>
#include <Framework/Event/Action/FRAWatch.h>


SYS_DEFINE_TYPE(FRAction, fr_action, SYS_TYPE_OBJECT);

SysBool fr_action_check(FRAction *self, FREvent *e) {
  FRActionClass *cls = FR_ACTION_GET_CLASS(self);

  return cls->check(self, e);
}

void fr_action_dispatch(FRAction *self, FREvent *e) {
  FRActionClass *cls = FR_ACTION_GET_CLASS(self);

  cls->dispatch(self, e);
}

SysBool fr_action_check_i(FRAction *self, FREvent *e) {
  return true;
}

FRAction *fr_action_ref(FRAction *self) {
  sys_object_ref(self);

  return self;
}

void fr_action_unbind_awatch(FRAction *self, SysList *action_link) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(action_link != NULL);

  self->awatch_list = sys_list_delete_link(self->awatch_list, action_link);
  sys_object_unref(action_link->data);
}

SysList* fr_action_bind_awatch(FRAction *self, FRAWatch *awatch) {
  sys_return_val_if_fail(self != NULL, NULL);
  sys_return_val_if_fail(awatch != NULL, NULL);

  self->awatch_list = sys_list_prepend(self->awatch_list, awatch);
  sys_object_ref(awatch);

  return self->awatch_list;
}

static void fr_action_dispatch_i(FRAction *self, FREvent *e) {
  sys_return_if_fail(self != NULL);

  FRAWatch *awatch;

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

const SysChar *fr_action_get_name(FRAction *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

void fr_action_set_name(FRAction *self, const SysChar *name) {
  sys_return_if_fail(self != NULL);

  sys_assert(self->name == NULL);

  self->name = sys_strdup(name);
}

/* object api */
void fr_action_create_i(FRAction *self) {
  self->awatch_list = NULL;
}

FRAction* fr_action_new(void) {
  return sys_object_new(FR_TYPE_ACTION, NULL);
}

FRAction *fr_action_new_I(void) {
  FRAction *o = fr_action_new();

  fr_action_set_name(o, "action");
  fr_action_create_i(o);

  return o;
}

static void fr_action_dispose(SysObject* o) {
  FRAction *self = FR_ACTION(o);

  sys_list_free_full(self->awatch_list, (SysDestroyFunc)_sys_object_unref);
  sys_clear_pointer(&self->name, sys_free);

  SYS_OBJECT_CLASS(fr_action_parent_class)->dispose(o);
}

static void fr_action_class_init(FRActionClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->create = fr_action_create_i;
  cls->check = fr_action_check_i;
  cls->dispatch = fr_action_dispatch_i;

  ocls->dispose = fr_action_dispose;
}

void fr_action_init(FRAction *self) {
}

FRAction* fr_action_get_static(void) {
  static FRAction *node = NULL;

  if(node != NULL) {
    return node;
  }

  node = fr_action_new_I();

  return node;
}
