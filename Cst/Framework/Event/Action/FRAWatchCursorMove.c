#include <Framework/Event/Action/FRAWatchCursorMove.h>
#include <Framework/Event/Base/FREventMouseKey.h>
#include <Framework/Event/Action/FRACursorMove.h>

SYS_DEFINE_TYPE(FRAWatchCursorMove, fr_awatch_cursor_move, FR_TYPE_AWATCH);


static SysBool fr_awatch_cursor_move_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchCursorMove *self = FR_AWATCH_CURSOR_MOVE(o);
  const FRRect *bound;
  FRACursorMove *mv;

  SysDouble x = 0, y = 0;

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  FREventMouseKey *mkey = FR_EVENT_MOUSEKEY(e);

  if(fr_event_mousekey_action(mkey) != FR_KEY_PRESS) {
    return false;
  }

  if(self->get_bound_func) {
    mv = FR_ACURSOR_MOVE(FR_ACURSOR_MOVE_STATIC);
    SysPointer user_data = fr_awatch_get_data(o);

    bound = self->get_bound_func(user_data);
    if(!fr_acursor_move_get_position(mv, &x, &y)) {
      return false;
    }

    if(!fr_rect_in_range(bound, (SysInt)x, (SysInt)y)) {
      return false;
    }
  }

  return true;
}

/* object api */
static void fr_awatch_cursor_move_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {

  FR_AWATCH_CLASS(fr_awatch_cursor_move_parent_class)->create(o, func_name, func, props);

  FRAWatchCursorMove *self = FR_AWATCH_CURSOR_MOVE(o);

  if(props->get_bound_func) {
    self->get_bound_func = props->get_bound_func;
  }
}

FRAWatchCursorMove* fr_awatch_cursor_move_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_CURSOR_MOVE, NULL);
}

static void fr_awatch_cursor_move_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_cursor_move_parent_class)->dispose(o);
}

static void fr_awatch_cursor_move_class_init(FRAWatchCursorMoveClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  wcls->create = fr_awatch_cursor_move_create_i;
  wcls->check = fr_awatch_cursor_move_check_i;

  ocls->dispose = fr_awatch_cursor_move_dispose;
}

void fr_awatch_cursor_move_init(FRAWatchCursorMove *self) {
  fr_awatch_set_action(FR_AWATCH(self), FR_ACURSOR_MOVE_STATIC);
}

