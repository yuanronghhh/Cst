#include <Framework/Event/Action/FrAMouseKey.h>
#include <Framework/Event/Base/FrEventMouseKey.h>
#include <Framework/Event/FrEvents.h>


SYS_DEFINE_TYPE(FrAMouseKey, fr_amouse_key, FR_TYPE_ACTION);


static SysBool fr_amouse_key_check_i(FrAction *self, FrEvent *e) {

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  return true;
}

static void fr_amouse_key_create_i(FrAction *o) {
  FR_ACTION_CLASS(fr_amouse_key_parent_class)->create(o);
}

/* object api */
FrAction* fr_amouse_key_new(void) {
  return sys_object_new(FR_TYPE_AMOUSE_KEY, NULL);
}

FrAction *fr_amouse_key_new_I(void) {
  FrAction *o = fr_amouse_key_new();

  fr_amouse_key_create_i(o);

  return o;
}

static void fr_amouse_key_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_amouse_key_parent_class)->dispose(o);
}

static void fr_amouse_key_class_init(FrAMouseKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrActionClass *acls = FR_ACTION_CLASS(cls);

  ocls->dispose = fr_amouse_key_dispose;

  acls->create = fr_amouse_key_create_i;
  acls->check = fr_amouse_key_check_i;
}

static void fr_amouse_key_init(FrAMouseKey *self) {
  fr_action_set_name(FR_ACTION(self), "mouse_key");
}

FrAction* fr_amouse_key_get_static(void) {
  static FrAction *node = NULL;

  fr_events_lock();

  if(node != NULL) {
    goto done;
  }

  node = fr_amouse_key_new_I();

done:
  fr_events_unlock();

  return node;
}
