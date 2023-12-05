#include <Framework/Event/Action/FRAWatchBuilder.h>

#include <Framework/Event/Action/FRAWatchMousePress.h>
#include <Framework/Event/Action/FRAWatchMouseRelease.h>
#include <Framework/Event/Action/FRAWatchKey.h>
#include <Framework/Event/Action/FRAWatchKeyPress.h>
#include <Framework/Event/Action/FRAWatchAny.h>
#include <Framework/Event/Action/FRAWatchCursorMove.h>
#include <Framework/Event/Action/FRAWatchRefresh.h>


SYS_DEFINE_TYPE(FRAWatchBuilder, fr_awatch_builder, SYS_TYPE_OBJECT);


void fr_awatch_builder_build_awatch_key(FRAWatchBuilder *self, FRAWatchKey *o) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(o != NULL);

  fr_awatch_key_set_key(o, self->key);
}

void fr_awatch_builder_build_awatch(FRAWatchBuilder *self, FRAWatch *o) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(o != NULL);

  fr_awatch_set_user_data(o, self->user_data);
  fr_awatch_set_func_name(o, self->func_name);
  fr_awatch_set_func(o, self->func);
}

void fr_awatch_builder_build_awatch_any(FRAWatchBuilder *self, FRAWatchAny *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_any_set_event_enum(o, self->event_enum);
}

void fr_awatch_builder_build_awatch_key_press(FRAWatchBuilder *self, FRAWatchKeyPress *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_key_press_set_key(o, self->key);
}

void fr_awatch_builder_build_awatch_mouse_press(FRAWatchBuilder *self, FRAWatchMousePress *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_mouse_press_set_bound_func(o, self->bound_func);
}

void fr_awatch_builder_build_awatch_mouse_release(FRAWatchBuilder *self, FRAWatchMouseRelease *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_mouse_release_set_bound_func(o, self->bound_func);
}

void fr_awatch_builder_build_awatch_cursor_move(FRAWatchBuilder *self, FRAWatchCursorMove *o) {
  sys_return_if_fail(self != NULL);

  fr_awatch_cursor_move_set_bound_func(o, self->bound_func);
}

void fr_awatch_builder_set_key(FRAWatchBuilder *self, SysInt key) {
  sys_return_if_fail(self != NULL);

  self->key = key;
}

SysInt fr_awatch_builder_get_key(FRAWatchBuilder *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->key;
}

void fr_awatch_builder_set_etype(FRAWatchBuilder *self, SysType etype) {
  sys_return_if_fail(self != NULL);

  self->etype = etype;
}

SysType fr_awatch_builder_get_etype(FRAWatchBuilder *self) {
  sys_return_val_if_fail(self != NULL, 0);

  return self->etype;
}

void fr_awatch_builder_set_event_enum(FRAWatchBuilder *self, SysInt event_enum) {
  sys_return_if_fail(self != NULL);

  self->event_enum = event_enum;
}

SysInt fr_awatch_builder_get_event_enum(FRAWatchBuilder *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->event_enum;
}

/* object api */

static void fr_awatch_builder_construct(FRAWatchBuilder* self, const SysChar *func_name, FREventFunc func) {
  self->func_name = sys_strdup(func_name);
  self->func = func;
}

static void fr_awatch_builder_dispose(SysObject* o) {
  FRAWatchBuilder *self = FR_AWATCH_BUILDER(o);

  sys_clear_pointer(&self->func_name, sys_free);

  self->func = NULL;
  self->user_data = NULL;
  self->etype = 0;
  self->key = -1;
  self->bound_func = NULL;

  SYS_OBJECT_CLASS(fr_awatch_builder_parent_class)->dispose(o);
}

FRAWatchBuilder *fr_awatch_builder_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_BUILDER, NULL);
}

FRAWatchBuilder *fr_awatch_builder_new_I(const SysChar *func_name, FREventFunc func) {
  FRAWatchBuilder *o = fr_awatch_builder_new();

  fr_awatch_builder_construct(o, func_name, func);

  return o;
}

static void fr_awatch_builder_class_init(FRAWatchBuilderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_awatch_builder_dispose;
}

static void fr_awatch_builder_init(FRAWatchBuilder *self) {
}
