#include <Framework/Event/Action/FRAWatchCursorMove.h>
#include <Framework/Event/FREventMF.h>
#include <Framework/Event/FRActionMF.h>

struct _FRAWatchCursorMovePrivate {
  FRGetBoundFunc get_bound_func;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAWatchCursorMove, fr_awatch_cursor_move, FR_TYPE_AWATCH);


static SysBool fr_awatch_cursor_move_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchCursorMove *self = FR_AWATCH_CURSOR_MOVE(o);
  FRAWatchCursorMovePrivate *priv = self->priv;
  SysInt x = 0, y = 0;
  FRRect bound;

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  FREventMouseKey *mkey = FR_EVENT_MOUSEKEY(e);

  if(fr_event_mousekey_action(mkey) != FR_KEY_PRESS) {
    return false;
  }

  if(priv->get_bound_func) {
    SysPointer user_data = fr_awatch_get_data(o);

    priv->get_bound_func(user_data, &bound);

    fr_acursor_move_get_position(FR_ACURSOR_MOVE_STATIC, &x, &y);

    if(!fr_rect_in_range(&bound, x, y)) {
      return false;
    }
  }


  return true;
}

/* object api */
static void fr_awatch_cursor_move_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {

  FR_AWATCH_CLASS(fr_awatch_cursor_move_parent_class)->create(o, user_data, func_name, func, props);

  FRAWatchCursorMove *self = FR_AWATCH_CURSOR_MOVE(o);
  FRAWatchCursorMovePrivate *priv = self->priv;

  if(props->get_bound_func) {
    priv->get_bound_func = props->get_bound_func;
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
  FRAWatchCursorMovePrivate *priv = self->priv = fr_awatch_cursor_move_get_private(self);

  fr_awatch_set_action(FR_AWATCH(self), FR_ACURSOR_MOVE_STATIC);
}

