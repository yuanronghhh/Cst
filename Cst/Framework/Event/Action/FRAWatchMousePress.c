#include <Framework/Event/Action/FRAWatchMousePress.h>
#include <Framework/Event/Base/FREventMouseKey.h>
#include <Framework/Event/Action/FRACursorMove.h>
#include <Framework/Event/Action/FRAMouseKey.h>
#include <Framework/Graph/FRGraph.h>


struct _FRAWatchMousePressPrivate {
  FRGetBoundFunc bound_func;
};

SYS_DEFINE_TYPE(FRAWatchMousePress, fr_awatch_mouse_press, FR_TYPE_AWATCH);


static SysBool fr_awatch_mouse_press_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchMousePress *self = FR_AWATCH_MOUSE_PRESS(o);
  FRACursorMove *acursor;
  SysDouble x = 0, y = 0;
  const FRRect *bound;

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  FREventMouseKey *mkey = FR_EVENT_MOUSEKEY(e);

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

  FRAWatchMousePress* nself = FR_AWATCH_MOUSE_PRESS(n);
  FRAWatchMousePress* oself = FR_AWATCH_MOUSE_PRESS(o);

  nself->bound_func = oself->bound_func;

  return n;
}

static void fr_awatch_mouse_press_construct_i(FRAWatch* o, FRAWatchBuilder *builder) {
  FR_AWATCH_CLASS(fr_awatch_mouse_press_parent_class)->construct(o, builder);

  FRAWatchMousePress *self = FR_AWATCH_MOUSE_PRESS(o);

  fr_awatch_builder_build_awatch_mouse_press(builder, self);
}

void fr_awatch_mouse_press_set_bound_func(FRAWatchMousePress *self, FRGetBoundFunc bound_func) {
  sys_return_if_fail(self != NULL);

  self->bound_func = bound_func;
}

FRGetBoundFunc fr_awatch_mouse_press_get_bound_func(FRAWatchMousePress *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->bound_func;
}

/* object api */
FRAWatchMousePress* fr_awatch_mouse_press_new(void) {
  return sys_object_new(FR_TYPE_AWATCH_MOUSE_PRESS, NULL);
}

static void fr_awatch_mouse_press_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_awatch_mouse_press_parent_class)->dispose(o);
}

static void fr_awatch_mouse_press_class_init(FRAWatchMousePressClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRAWatchClass *wcls = FR_AWATCH_CLASS(cls);

  wcls->construct = fr_awatch_mouse_press_construct_i;
  wcls->check = fr_awatch_mouse_press_check_i;

  ocls->dispose = fr_awatch_mouse_press_dispose;
  ocls->dclone = fr_awatch_mouse_press_dclone_i;
}

void fr_awatch_mouse_press_init(FRAWatchMousePress *self) {

  fr_awatch_set_action(FR_AWATCH(self), FR_AMOUSE_KEY_STATIC);
}

