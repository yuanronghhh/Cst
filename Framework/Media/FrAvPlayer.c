#include <Framework/Media/FrAvPlayer.h>

SYS_DEFINE_TYPE(FrAvPlayer, fr_av_player, FR_TYPE_PLAYER);

/* object api */
static void fr_av_player_construct_i(FrPlayer *self, FrAvPlayerContext *info) {
}

FrPlayer* fr_av_player_new(void) {
  return sys_object_new(FR_TYPE_AV_PLAYER, NULL);
}

FrPlayer *fr_av_player_new_I(FrAvPlayerContext *info) {
  FrPlayer *o = fr_av_player_new();

  fr_av_player_construct_i(o, info);

  return o;
}

static void fr_av_player_dispose(SysObject* o) {
  FrAvPlayer *self = FR_AV_PLAYER(o);

  UNUSED(self);

  SYS_OBJECT_CLASS(fr_av_player_parent_class)->dispose(o);
}

static void fr_av_player_class_init(FrAvPlayerClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_av_player_dispose;
}

static void fr_av_player_init(FrAvPlayer* self) {
}
