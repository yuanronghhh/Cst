#ifndef __FR_ACURSOR_MOVE_H__
#define __FR_ACURSOR_MOVE_H__

#include <Framework/Event/Action/FRAction.h>


SYS_BEGIN_DECLS

#define FR_ACURSOR_MOVE_STATIC (fr_acursor_move_get_static())
#define FR_TYPE_ACURSOR_MOVE (fr_acursor_move_get_type())
#define FR_ACURSOR_MOVE(o) ((FRACursorMove *)sys_object_cast_check(o, FR_TYPE_ACURSOR_MOVE))
#define FR_ACURSOR_MOVE_CLASS(o) ((FRACursorMoveClass *)sys_class_cast_check(o, FR_TYPE_ACURSOR_MOVE))
#define FR_ACURSOR_MOVE_GET_CLASS(o) sys_instance_get_class(o, FRACursorMoveClass)


struct _FRACursorMoveClass {
  FRActionClass parent;
};

struct _FRACursorMove {
  FRAction parent;

  /* <private> */
  SysDouble x;
  SysDouble y;
};

SYS_API FRAction *fr_acursor_move_get_static(void);
SYS_API FRAction* fr_acursor_move_new(void);
SYS_API SysType fr_acursor_move_get_type(void);
SYS_API SysBool fr_acursor_move_get_position (FRACursorMove *self, SysDouble *x, SysDouble *y);

SYS_END_DECLS

#endif
