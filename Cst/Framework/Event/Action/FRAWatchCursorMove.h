#ifndef __FR_AWATCH_CURSOR_MOVE_H__
#define __FR_AWATCH_CURSOR_MOVE_H__

#include <Framework/Event/Action/FRAWatch.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_CURSOR_MOVE (fr_awatch_cursor_move_get_type())
#define FR_AWATCH_CURSOR_MOVE(o) ((FRAWatchCursorMove* )sys_object_cast_check(o, FR_TYPE_AWATCH_CURSOR_MOVE))
#define FR_AWATCH_CURSOR_MOVE_CLASS(o) ((FRAWatchCursorMoveClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_CURSOR_MOVE))
#define FR_AWATCH_CURSOR_MOVE_GET_CLASS(o) sys_instance_get_class(o, FRAWatchCursorMoveClass)

struct _FRAWatchCursorMoveClass {
  FRAWatchClass parent;
};

struct _FRAWatchCursorMove {
  FRAWatch parent;

  /* <private> */
  FRGetBoundFunc bound_func;
};

SYS_API FRAWatchCursorMove* fr_awatch_cursor_move_new(void);
SYS_API SysType fr_awatch_cursor_move_get_type(void);

void fr_awatch_cursor_move_set_bound_func(FRAWatchCursorMove *self, FRGetBoundFunc bound_func);
FRGetBoundFunc fr_awatch_cursor_move_get_bound_func(FRAWatchCursorMove *self);

SYS_END_DECLS

#endif
