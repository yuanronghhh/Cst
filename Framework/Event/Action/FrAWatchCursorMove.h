#ifndef __FR_AWATCH_CURSOR_MOVE_H__
#define __FR_AWATCH_CURSOR_MOVE_H__

#include <Framework/Event/Action/FrAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_CURSOR_MOVE (fr_awatch_cursor_move_get_type())
#define FR_AWATCH_CURSOR_MOVE(o) ((FrAWatchCursorMove* )sys_object_cast_check(o, FR_TYPE_AWATCH_CURSOR_MOVE))
#define FR_AWATCH_CURSOR_MOVE_CLASS(o) ((FrAWatchCursorMoveClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_CURSOR_MOVE))
#define FR_AWATCH_CURSOR_MOVE_GET_CLASS(o) sys_instance_get_class(o, FrAWatchCursorMoveClass)

struct _FrAWatchCursorMoveClass {
  FrAWatchClass parent;
};

struct _FrAWatchCursorMove {
  FrAWatch parent;

  /* <private> */
  FrGetBoundFunc bound_func;
};

SYS_API FrAWatchCursorMove* fr_awatch_cursor_move_new(void);
SYS_API SysType fr_awatch_cursor_move_get_type(void);

void fr_awatch_cursor_move_set_bound_func(FrAWatchCursorMove *self, FrGetBoundFunc bound_func);
FrGetBoundFunc fr_awatch_cursor_move_get_bound_func(FrAWatchCursorMove *self);

SYS_END_DECLS

#endif
