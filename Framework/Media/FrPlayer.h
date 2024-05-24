#ifndef __FR_PLAYER_H__
#define __FR_PLAYER_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_PLAYER (fr_player_get_type())
#define FR_PLAYER(o) ((FrPlayer* )sys_object_cast_check(o, FR_TYPE_PLAYER))
#define FR_PLAYER_CLASS(o) ((FrPlayerClass *)sys_class_cast_check(o, FR_TYPE_PLAYER))
#define FR_PLAYER_GET_CLASS(o) sys_instance_get_class(o, FrPlayerClass)

struct _FrPlayerClass {
  SysObjectClass parent;
};

struct _FrPlayer {
  SysObject parent;

  /* <private> */
};

SYS_API SysType fr_player_get_type(void);
SYS_API FrPlayer *fr_player_new(void);

SYS_API FrPlayer *fr_player_new_I(void);

SYS_END_DECLS

#endif
