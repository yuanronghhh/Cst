#include <Framework/Event/Base/FrEventCursorMove.h>

struct _FrEventCursorMovePrivate {
  SysDouble xpos;
  SysDouble ypos;
};

SYS_DEFINE_TYPE(FrEventCursorMove, fr_event_cursor_move, FR_TYPE_EVENT);


void fr_event_cursor_move_position(FrEventCursorMove *self, SysDouble *x, SysDouble *y) {
  sys_return_if_fail(self != NULL);


  *x = self->xpos;
  *y = self->ypos;
}

/* object api */
static void fr_event_cursor_move_construct(FrEvent *o, FrWindow *window, SysDouble xpos, SysDouble ypos) {
  FR_EVENT_CLASS(fr_event_cursor_move_parent_class)->construct(o, window);

  FrEventCursorMove* self = FR_EVENT_CURSOR_MOVE(o);

  self->xpos = xpos;
  self->ypos = ypos;
}

FrEvent* fr_event_cursor_move_new(void) {
  return sys_object_new(FR_TYPE_EVENT_CURSOR_MOVE, NULL);
}

FrEvent *fr_event_cursor_move_new_I(FrWindow *window, SysDouble xpos, SysDouble ypos) {
  FrEvent *o = fr_event_cursor_move_new();

  fr_event_cursor_move_construct(o, window, xpos, ypos);

  return o;
}

static void fr_event_cursor_move_dispose(SysObject* o) {


}

static void fr_event_cursor_move_class_init(FrEventCursorMoveClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_event_cursor_move_dispose;
}

void fr_event_cursor_move_init(FrEventCursorMove *self) {
}

