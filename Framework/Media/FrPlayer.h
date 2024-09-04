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

  SysInt (*init) (FrPlayer *self);
  SysInt (*started) (FrPlayer *self);
  SysInt (*process) (FrPlayer *self);
  SysInt (*stop) (FrPlayer *self);
};

struct _FrPlayer {
  SysObject parent;

  /* <private> */
  FR_JOB_STATE_ENUM state;
  SysUInt64 loop_count;
};

SYS_API SysType fr_player_get_type(void);
SYS_API FrPlayer *fr_player_new(void);

SYS_API FrPlayer *fr_player_new_I(void);
SYS_API SysInt fr_player_run(FrPlayer* self);

void fr_player_set_state(FrPlayer *self, FR_JOB_STATE_ENUM state);
FR_JOB_STATE_ENUM fr_player_get_state(FrPlayer *self);

SYS_END_DECLS

#endif
