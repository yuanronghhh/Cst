#ifndef __FR_EVENT_CURSOR_MOVE_H__
#define __FR_EVENT_CURSOR_MOVE_H__

#include <Framework/Event/Base/FREvent.h>

SYS_BEGIN_DECLS

#define FR_TYPE_EVENT_CURSOR_MOVE (fr_event_cursor_move_get_type())
#define FR_EVENT_CURSOR_MOVE(o) ((FREventCursorMove* )sys_object_cast_check(o, FR_TYPE_EVENT_CURSOR_MOVE))
#define FR_EVENT_CURSOR_MOVE_CLASS(o) ((FREventCursorMoveClass *)sys_class_cast_check(o, FR_TYPE_EVENT_CURSOR_MOVE))
#define FR_EVENT_CURSOR_MOVE_GET_CLASS(o) sys_instance_get_class(o, FREventCursorMoveClass)

struct _FREventCursorMoveClass {
  FREventClass parent;
};

struct _FREventCursorMove {
  FREvent parent;

  FREventCursorMovePrivate *priv;
};

SYS_API SysType fr_event_cursor_move_get_type(void);
SYS_API FREvent * fr_event_cursor_move_new_I(FRWindow * window, SysDouble xpos, SysDouble ypos);
SYS_API void fr_event_cursor_move_position(FREventCursorMove *self, SysDouble *x, SysDouble *y) ;

SYS_END_DECLS

#endif

