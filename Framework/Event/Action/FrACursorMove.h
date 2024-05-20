#ifndef __FR_ACURSOR_MOVE_H__
#define __FR_ACURSOR_MOVE_H__

#include <Framework/Event/Action/FrAction.h>


SYS_BEGIN_DECLS

#define FR_ACURSOR_MOVE_STATIC (fr_acursor_move_get_static())
#define FR_TYPE_ACURSOR_MOVE (fr_acursor_move_get_type())
#define FR_ACURSOR_MOVE(o) ((FrACursorMove *)sys_object_cast_check(o, FR_TYPE_ACURSOR_MOVE))
#define FR_ACURSOR_MOVE_CLASS(o) ((FrACursorMoveClass *)sys_class_cast_check(o, FR_TYPE_ACURSOR_MOVE))
#define FR_ACURSOR_MOVE_GET_CLASS(o) sys_instance_get_class(o, FrACursorMoveClass)


struct _FrACursorMoveClass {
  FrActionClass parent;
};

struct _FrACursorMove {
  FrAction parent;

  /* <private> */
  SysDouble x;
  SysDouble y;
};

SYS_API FrAction *fr_acursor_move_get_static(void);
SYS_API FrAction* fr_acursor_move_new(void);
SYS_API SysType fr_acursor_move_get_type(void);
SYS_API SysBool fr_acursor_move_get_position (FrACursorMove *self, SysDouble *x, SysDouble *y);

SYS_END_DECLS

#endif
