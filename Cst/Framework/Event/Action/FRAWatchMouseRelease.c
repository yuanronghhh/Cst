#include <Framework/Event/Action/FRAWatchMouseRelease.h>
#include <Framework/Event/Base/FREventMouseKey.h>
#include <Framework/Event/Action/FRACursorMove.h>
#include <Framework/Event/Action/FRAMouseKey.h>
#include <Framework/Graph/FRGraph.h>


SYS_DEFINE_TYPE(FRAWatchMouseRelease, fr_awatch_mouse_release, FR_TYPE_AWATCH);


static SysBool fr_awatch_mouse_release_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchMouseRelease *self = FR_AWATCH_MOUSE_RELEASE(o);
  FRACursorMove *acursor;
  SysDouble x = 0, y = 0;
  const FRRect *bound;

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  FREventMouseKey *mkey = FR_EVENT_MOUSEKEY(e);

  if(fr_event_mousekey_action(mkey) != FR_KEY_RELEASE) {
    return false;
  }

  if(self->bound_func) {
    acursor = FR_ACURSOR_MOVE(FR_ACURSOR_MOVE_STATIC);

    SysPointer user_data = fr_awatch_get_data(o);
    bound = self->bound_func(user_data);

    if(!fr_acursor_move_get_position(acursor, &x, &y)) {
      return false;
    }

    if(!fr_rect_in_range(bound, (SysInt)x, (SysInt)y)) {
      return false;
    }
  }

  return true;
}

static SysObject* fr_awatch_mouse_release_dclone_i(SysObject* o) {
  SysObject* nwatch = SYS_OBJECT_CLASS(fr_awatch_mouse_release_parent_class)->dclone(o);

  FRAWatchMouseRelease *nself = FR_AWATCH_MOUSE_RELEASE(nwatch);
  FRAWatchMouseRelease *oself = FR_AWATCH_MOUSE_RELEASE(o);

  nself->bound_func = oself->bound_func;

  return nwatch;
}

static void fr_awatch_mouse_release_construct_i(FRAWatch* o, FRAWatchBuilder *builder) {
  FR_AWATCH_CLASS(fr_awatch_mouse_release_parent_class)->construct(o, builder);

  FRAWatchMouseRelease *self = FR_AWATCH_MOUSE_RELEASE(o);

  fr_awatch_builder_build_awatch_mouse_release(builder, self);
}

void fr_awatch_mouse_release_set_bound_func(FRAWatchMouseRelease *self, FRGetBoundFunc bound_func) {
  sys_return_if_fail(self != NULL);

  self->bound_func = bound_func;
}

FRGetBoundFunc fr_awatch_mouse_release_get_bound_func(FRAWatchMouseRelease *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->bound_func;
}


/* object api */
FRAWatchMouseRelease* fr_awatch_mouse_release_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_MOUSE_RELEASE, NULL);
}

static void fr_awatch_mouse_release_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_mouse_release_parent_class)->dispose(o);
}

static void fr_awatch_mouse_release_class_init(FRAWatchMouseReleaseClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  wcls->construct = fr_awatch_mouse_release_construct_i;
  wcls->check = fr_awatch_mouse_release_check_i;

  ocls->dclone = fr_awatch_mouse_release_dclone_i;
  ocls->dispose = fr_awatch_mouse_release_dispose;
}

void fr_awatch_mouse_release_init(FRAWatchMouseRelease *self) {
  fr_awatch_set_action(FR_AWATCH(self), FR_AMOUSE_KEY_STATIC);
}

