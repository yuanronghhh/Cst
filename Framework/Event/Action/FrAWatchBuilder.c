#include <Framework/Event/Action/FrAWatchBuilder.h>

#include <Framework/Event/Action/FrAWatchMousePress.h>
#include <Framework/Event/Action/FrAWatchMouseRelease.h>
#include <Framework/Event/Action/FrAWatchKey.h>
#include <Framework/Event/Action/FrAWatchKeyPress.h>
#include <Framework/Event/Action/FrAWatchAny.h>
#include <Framework/Event/Action/FrAWatchCursorMove.h>
#include <Framework/Event/Action/FrAWatchRefresh.h>


SYS_DEFINE_TYPE(FrAWatchBuilder, fr_awatch_builder, SYS_TYPE_OBJECT);


void fr_awatch_builder_build_awatch_key(FrAWatchBuilder *self, FrAWatchKey *o) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(o != NULL);

  fr_awatch_key_set_key(o, self->key);
}

void fr_awatch_builder_build_awatch(FrAWatchBuilder *self, FrAWatch *o) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(o != NULL);

  fr_awatch_set_user_data(o, self->user_data);
  fr_awatch_set_func_name(o, self->func_name);
  fr_awatch_set_func(o, self->func);
}

void fr_awatch_builder_build_awatch_any(FrAWatchBuilder *self, FrAWatchAny *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_any_set_event_enum(o, self->event_enum);
}

void fr_awatch_builder_build_awatch_key_press(FrAWatchBuilder *self, FrAWatchKeyPress *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_key_press_set_key(o, self->key);
}

void fr_awatch_builder_build_awatch_mouse_press(FrAWatchBuilder *self, FrAWatchMousePress *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_mouse_press_set_bound_func(o, self->bound_func);
}

void fr_awatch_builder_build_awatch_mouse_release(FrAWatchBuilder *self, FrAWatchMouseRelease *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_mouse_release_set_bound_func(o, self->bound_func);
}

void fr_awatch_builder_build_awatch_cursor_move(FrAWatchBuilder *self, FrAWatchCursorMove *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_cursor_move_set_bound_func(o, self->bound_func);
}

void fr_awatch_builder_set_key(FrAWatchBuilder *self, SysInt key) {
  sys_return_if_fail(self != NULL);

  self->key = key;
}

SysInt fr_awatch_builder_get_key(FrAWatchBuilder *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->key;
}

void fr_awatch_builder_set_etype(FrAWatchBuilder *self, SysType etype) {
  sys_return_if_fail(self != NULL);

  self->etype = etype;
}

SysType fr_awatch_builder_get_etype(FrAWatchBuilder *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->etype;
}

void fr_awatch_builder_set_event_enum(FrAWatchBuilder *self, SysInt event_enum) {
  sys_return_if_fail(self != NULL);

  self->event_enum = event_enum;
}

SysInt fr_awatch_builder_get_event_enum(FrAWatchBuilder *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->event_enum;
}

/* object api */

static void fr_awatch_builder_construct(FrAWatchBuilder* self, const SysChar *func_name, FrEventFunc func) {
  self->func_name = sys_strdup(func_name);
  self->func = func;
}

static void fr_awatch_builder_dispose(SysObject* o) {
  FrAWatchBuilder *self = FR_AWATCH_BUILDER(o);

  sys_clear_pointer(&self->func_name, sys_free);

  self->func = NULL;
  self->user_data = NULL;
  self->etype = 0;
  self->key = -1;
  self->bound_func = NULL;

  SYS_OBJECT_CLASS(fr_awatch_builder_parent_class)->dispose(o);
}

FrAWatchBuilder *fr_awatch_builder_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_BUILDER, NULL);
}

FrAWatchBuilder *fr_awatch_builder_new_I(const SysChar *func_name, FrEventFunc func) {
  FrAWatchBuilder *o = fr_awatch_builder_new();

  fr_awatch_builder_construct(o, func_name, func);

  return o;
}

static void fr_awatch_builder_class_init(FrAWatchBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_awatch_builder_dispose;
}

static void fr_awatch_builder_init(FrAWatchBuilder *self) {
}
