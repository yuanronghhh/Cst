#include <Framework/Event/Base/FREventCursorMove.h>

struct _FREventCursorMovePrivate {
  SysDouble xpos;
  SysDouble ypos;
};

SYS_DEFINE_TYPE(FREventCursorMove, fr_event_cursor_move, FR_TYPE_EVENT);


void fr_event_cursor_move_position(FREventCursorMove *self, SysDouble *x, SysDouble *y) {
  sys_return_if_fail(self != NULL);


  *x = self->xpos;
  *y = self->ypos;
}

/* object api */
static void fr_event_cursor_move_construct(FREvent *o, FRWindow *window, SysDouble xpos, SysDouble ypos) {
  FR_EVENT_CLASS(fr_event_cursor_move_parent_class)->construct(o, window);

  FREventCursorMove* self = FR_EVENT_CURSOR_MOVE(o);

  self->xpos = xpos;
  self->ypos = ypos;
}

FREvent* fr_event_cursor_move_new(void) {
  return sys_object_new(FR_TYPE_EVENT_CURSOR_MOVE, NULL);
}

FREvent *fr_event_cursor_move_new_I(FRWindow *window, SysDouble xpos, SysDouble ypos) {
  FREvent *o = fr_event_cursor_move_new();

  fr_event_cursor_move_construct(o, window, xpos, ypos);

  return o;
}

static void fr_event_cursor_move_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_event_cursor_move_parent_class)->dispose(o);
}

static void fr_event_cursor_move_class_init(FREventCursorMoveClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_event_cursor_move_dispose;
}

void fr_event_cursor_move_init(FREventCursorMove *self) {
}

