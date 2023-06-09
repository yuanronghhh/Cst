#include <Framework/Event/Action/FRAWatchMousePress.h>
#include <Framework/Event/Base/FREventMouseKey.h>
#include <Framework/Event/Action/FRACursorMove.h>
#include <Framework/Event/Action/FRAMouseKey.h>
#include <Framework/Graph/FRGraph.h>


struct _FRAWatchMousePressPrivate {
  FRGetBoundFunc get_bound_func;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAWatchMousePress, fr_awatch_mouse_press, FR_TYPE_AWATCH);


static SysBool fr_awatch_mouse_press_check_i(FRAWatch *o, FREvent *e) {
  FRAWatchMousePress *self = FR_AWATCH_MOUSE_PRESS(o);
  FRAWatchMousePressPrivate *priv = self->priv;
  FRACursorMove *acursor;
  SysDouble x = 0, y = 0;
  FRRect bound;

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  FREventMouseKey *mkey = FR_EVENT_MOUSEKEY(e);

  if(fr_event_mousekey_action(mkey) != FR_KEY_PRESS) {
    return false;
  }

  if(priv->get_bound_func) {
    acursor = FR_ACURSOR_MOVE(FR_ACURSOR_MOVE_STATIC);

    SysPointer user_data = fr_awatch_get_data(o);
    priv->get_bound_func(user_data, &bound);

    fr_acursor_move_get_position(acursor, &x, &y);

    // sys_debug_N("<%d,%d,%d,%d> %lf,%lf,%d", bound.x, bound.y, bound.width, bound.height, x, y, fr_rect_in_range(&bound, x, y));
    if(!fr_rect_in_range(&bound, (SysInt)x, (SysInt)y)) {
      return false;
    }
  }

  return true;
}

static FRAWatch* fr_awatch_mouse_press_dclone_i(FRAWatch* old) {
  FRAWatch* nw = FR_AWATCH_CLASS(fr_awatch_mouse_press_parent_class)->dclone(old);

  FRAWatchMousePressPrivate *npriv = FR_AWATCH_MOUSE_PRESS(nw)->priv;
  FRAWatchMousePressPrivate *opriv = FR_AWATCH_MOUSE_PRESS(old)->priv;

  npriv->get_bound_func = opriv->get_bound_func;

  return nw;
}

static void fr_awatch_mouse_press_create_i(FRAWatch* o, const SysChar *func_name, FREventFunc func, FRAWatchProps *props) {
  FR_AWATCH_CLASS(fr_awatch_mouse_press_parent_class)->create(o, func_name, func, props);

  FRAWatchMousePress *self = FR_AWATCH_MOUSE_PRESS(o);
  FRAWatchMousePressPrivate *priv = self->priv;

  if(props->get_bound_func) {
    priv->get_bound_func = props->get_bound_func;
  }
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

  wcls->create = fr_awatch_mouse_press_create_i;
  wcls->dclone = fr_awatch_mouse_press_dclone_i;
  wcls->check = fr_awatch_mouse_press_check_i;

  ocls->dispose = fr_awatch_mouse_press_dispose;
}

void fr_awatch_mouse_press_init(FRAWatchMousePress *self) {
  self->priv = fr_awatch_mouse_press_get_private(self);

  fr_awatch_set_action(FR_AWATCH(self), FR_AMOUSE_KEY_STATIC);
}

