#include <Framework/Event/Action/FrAWatchCursorMove.h>

#include <Framework/Event/Base/FrEventMouseKey.h>
#include <Framework/Event/Action/FrACursorMove.h>

SYS_DEFINE_TYPE(FrAWatchCursorMove, fr_awatch_cursor_move, FR_TYPE_AWATCH);


static SysBool fr_awatch_cursor_move_check_i(FrAWatch *o, FrEvent *e) {
  FrAWatchCursorMove *self = FR_AWATCH_CURSOR_MOVE(o);
  const FrBound *bound;
  FrACursorMove *mv;

  SysDouble x = 0, y = 0;

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  FrEventMouseKey *mkey = FR_EVENT_MOUSEKEY(e);

  if(fr_event_mousekey_action(mkey) != FR_KEY_PRESS) {
    return false;
  }

  if(self->bound_func) {
    mv = FR_ACURSOR_MOVE(FR_ACURSOR_MOVE_STATIC);
    SysPointer user_data = fr_awatch_get_data(o);

    bound = self->bound_func(user_data);
    if(!fr_acursor_move_get_position(mv, &x, &y)) {
      return false;
    }

    if(!fr_rect_in_range(bound, (SysInt)x, (SysInt)y)) {
      return false;
    }
  }

  return true;
}

void fr_awatch_cursor_move_set_bound_func(FrAWatchCursorMove *self, FrGetBoundFunc bound_func) {
  sys_return_if_fail(self != NULL);

  self->bound_func = bound_func;
}

FrGetBoundFunc fr_awatch_cursor_move_get_bound_func(FrAWatchCursorMove *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->bound_func;
}

/* object api */
static void fr_awatch_cursor_move_construct_i(FrAWatch* o, FrAWatchBuilder *builder) {

  FR_AWATCH_CLASS(fr_awatch_cursor_move_parent_class)->construct(o, builder);

  FrAWatchCursorMove *self = FR_AWATCH_CURSOR_MOVE(o);

  fr_awatch_builder_build_awatch_cursor_move(builder, self);
}

FrAWatchCursorMove* fr_awatch_cursor_move_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_CURSOR_MOVE, NULL);
}

static void fr_awatch_cursor_move_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_cursor_move_parent_class)->dispose(o);
}

static void fr_awatch_cursor_move_class_init(FrAWatchCursorMoveClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  wcls->construct = fr_awatch_cursor_move_construct_i;
  wcls->check = fr_awatch_cursor_move_check_i;

  ocls->dispose = fr_awatch_cursor_move_dispose;
}

void fr_awatch_cursor_move_init(FrAWatchCursorMove *self) {
  fr_awatch_set_action(FR_AWATCH(self), FR_ACURSOR_MOVE_STATIC);
}

