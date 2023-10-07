#include <Framework/Event/Action/FRACursorMove.h>
#include <Framework/Event/Base/FREventCursorMove.h>


SYS_DEFINE_TYPE(FRACursorMove, fr_acursor_move, FR_TYPE_ACTION);

SysBool fr_acursor_move_get_position (FRACursorMove *self, SysDouble *x, SysDouble *y) {
  sys_return_val_if_fail(self != NULL, false);

  if(self->x == 0 || self->y == 0) {
    return false;
  }

  *x = self->x;
  *y = self->y;

  return true;
}

static void fr_acursor_move_create_i (FRAction *o) {

  FR_ACTION_CLASS(fr_acursor_move_parent_class)->create(o);
}

static SysBool fr_acursor_move_check_i (FRAction *self, FREvent *e) {
  if(!fr_event_is(e, FR_TYPE_EVENT_CURSOR_MOVE)) {
    return false;
  }

  return true;
}

static void fr_acursor_move_dispatch_i (FRAction *o, FREvent *e) {
  FRACursorMove *self = FR_ACURSOR_MOVE(o);
  FREventCursorMove *ecur = FR_EVENT_CURSOR_MOVE(e);

  fr_event_cursor_move_position(ecur, &self->x, &self->y);

  FR_ACTION_CLASS(fr_acursor_move_parent_class)->dispatch(o, e);
}

/* object api */
FRAction* fr_acursor_move_new(void) {
  return sys_object_new(FR_TYPE_ACURSOR_MOVE, NULL);
}

FRAction *fr_acursor_move_new_I(void) {
  FRAction *o = fr_acursor_move_new();

  fr_acursor_move_create_i(o);

  return o;
}

static void fr_acursor_move_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_acursor_move_parent_class)->dispose(o);
}

static void fr_acursor_move_class_init(FRACursorMoveClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRActionClass *acls = FR_ACTION_CLASS(cls);

  acls->create = fr_acursor_move_create_i;
  acls->check = fr_acursor_move_check_i;
  acls->dispatch = fr_acursor_move_dispatch_i;

  ocls->dispose = fr_acursor_move_dispose;
}

static void fr_acursor_move_init(FRACursorMove *self) {

  fr_action_set_name(FR_ACTION(self), "cursor_move");
}

FRAction *fr_acursor_move_get_static(void) {
  static FRAction *node = NULL;

  if(node != NULL) {
    return node;
  }

  node = fr_acursor_move_new_I();

  return node;
}
