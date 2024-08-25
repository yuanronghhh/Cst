#include <Framework/Event/Action/FrAWatchMousePress.h>
#include <Framework/Event/Base/FrEventMouseKey.h>
#include <Framework/Event/Action/FrACursorMove.h>
#include <Framework/Event/Action/FrAMouseKey.h>
#include <Framework/Graph/FrGraph.h>


struct _FrAWatchMousePressPrivate {
  FrGetBoundFunc bound_func;
};

SYS_DEFINE_TYPE(FrAWatchMousePress, fr_awatch_mouse_press, FR_TYPE_AWATCH);


static SysBool fr_awatch_mouse_press_check_i(FrAWatch *o, FrEvent *e) {
  FrAWatchMousePress *self = FR_AWATCH_MOUSE_PRESS(o);
  FrACursorMove *acursor;
  SysDouble x = 0, y = 0;
  const FrBound *bound;

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  FrEventMouseKey *mkey = FR_EVENT_MOUSEKEY(e);

  if(fr_event_mousekey_action(mkey) != FR_KEY_PRESS) {
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

static SysObject* fr_awatch_mouse_press_dclone_i(SysObject* o) {
  sys_return_val_if_fail(o != NULL, NULL);

  SysObject* n = SYS_OBJECT_CLASS(fr_awatch_mouse_press_parent_class)->dclone(o);

  FrAWatchMousePress* nself = FR_AWATCH_MOUSE_PRESS(n);
  FrAWatchMousePress* oself = FR_AWATCH_MOUSE_PRESS(o);

  nself->bound_func = oself->bound_func;

  return n;
}

static void fr_awatch_mouse_press_construct_i(FrAWatch* o, FrAWatchBuilder *builder) {
  FR_AWATCH_CLASS(fr_awatch_mouse_press_parent_class)->construct(o, builder);

  FrAWatchMousePress *self = FR_AWATCH_MOUSE_PRESS(o);

  fr_awatch_builder_build_awatch_mouse_press(builder, self);
}

void fr_awatch_mouse_press_set_bound_func(FrAWatchMousePress *self, FrGetBoundFunc bound_func) {
  sys_return_if_fail(self != NULL);

  self->bound_func = bound_func;
}

FrGetBoundFunc fr_awatch_mouse_press_get_bound_func(FrAWatchMousePress *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->bound_func;
}

/* object api */
FrAWatchMousePress* fr_awatch_mouse_press_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_MOUSE_PRESS, NULL);
}

static void fr_awatch_mouse_press_dispose(SysObject* o) {


}

static void fr_awatch_mouse_press_class_init(FrAWatchMousePressClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  wcls->construct = fr_awatch_mouse_press_construct_i;
  wcls->check = fr_awatch_mouse_press_check_i;

  ocls->dispose = fr_awatch_mouse_press_dispose;
  ocls->dclone = fr_awatch_mouse_press_dclone_i;
}

void fr_awatch_mouse_press_init(FrAWatchMousePress *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_AMOUSE_KEY_STATIC);
}

