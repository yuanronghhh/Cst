#include <Framework/Media/FrPlayer.h>

SYS_DEFINE_TYPE(FrPlayer, fr_player, SYS_TYPE_OBJECT);

/* object api */
static void fr_player_construct_i(FrPlayer *self) {

}

FrPlayer* fr_player_new(void) {
  return sys_object_new(FR_TYPE_PLAYER, NULL);
}

FrPlayer *fr_player_new_I(void) {
  FrPlayer *o = fr_player_new();

  fr_player_construct_i(o);

  return o;
}

static void fr_player_dispose(SysObject* o) {
  FrPlayer *self = FR_PLAYER(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_player_parent_class)->dispose(o);
}

static void fr_player_class_init(FrPlayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_player_dispose;
}

void fr_player_init(FrPlayer* self) {
}
