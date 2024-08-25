#include <Framework/Event/Action/FrACursorMove.h>
#include <Framework/Event/Base/FrEventCursorMove.h>
#include <Framework/Event/FrEvents.h>


SYS_DEFINE_TYPE(FrACursorMove, fr_acursor_move, FR_TYPE_ACTION);

SysBool fr_acursor_move_get_position (FrACursorMove *self, SysDouble *x, SysDouble *y) {
  sys_return_val_if_fail(self != NULL, false);

  if(self->x == 0 || self->y == 0) {
    return false;
  }

  *x = self->x;
  *y = self->y;

  return true;
}

static void fr_acursor_move_create_i (FrAction *o, FrActionContext *info) {
  info->name = "cursor_move";
  FR_ACTION_CLASS(fr_acursor_move_parent_class)->create(o, info);
}

static SysBool fr_acursor_move_check_i (FrAction *self, FrEvent *e) {
  if(!fr_event_is(e, FR_TYPE_EVENT_CURSOR_MOVE)) {
    return false;
  }

  return true;
}

static void fr_acursor_move_dispatch_i (FrAction *o, FrEvent *e) {
  FrACursorMove *self = FR_ACURSOR_MOVE(o);
  FrEventCursorMove *ecur = FR_EVENT_CURSOR_MOVE(e);

  fr_event_cursor_move_position(ecur, &self->x, &self->y);

  FR_ACTION_CLASS(fr_acursor_move_parent_class)->dispatch(o, e);
}

/* object api */
FrAction* fr_acursor_move_new(void) {
  return sys_object_new(FR_TYPE_ACURSOR_MOVE, NULL);
}

FrAction *fr_acursor_move_new_I(FrActionContext *info) {
  FrAction *o = fr_acursor_move_new();

  fr_acursor_move_create_i(o, info);

  return o;
}

static void fr_acursor_move_dispose(SysObject* o) {


}

static void fr_acursor_move_class_init(FrACursorMoveClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrActionClass *acls = FR_ACTION_CLASS(cls);

  acls->create = fr_acursor_move_create_i;
  acls->check = fr_acursor_move_check_i;
  acls->dispatch = fr_acursor_move_dispatch_i;

  ocls->dispose = fr_acursor_move_dispose;
}

static void fr_acursor_move_init(FrACursorMove *self) {
}

FrAction *fr_acursor_move_get_static(void) {
  static FrAction *node = NULL;

  fr_events_lock();
  if(node != NULL) {
    goto done;
  }

  FrActionContext info = {0};
  node = fr_acursor_move_new_I(&info);

done:
  fr_events_unlock();
  return node;
}
