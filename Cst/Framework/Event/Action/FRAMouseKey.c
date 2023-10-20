#include <Framework/Event/Action/FRAMouseKey.h>
#include <Framework/Event/Base/FREventMouseKey.h>


SYS_DEFINE_TYPE(FRAMouseKey, fr_amouse_key, FR_TYPE_ACTION);



static SysBool fr_amouse_key_check_i(FRAction *self, FREvent *e) {

  if(!fr_event_is(e, FR_TYPE_EVENT_MOUSEKEY)) {
    return false;
  }

  return true;
}

static void fr_amouse_key_create_i(FRAction *o) {
  FR_ACTION_CLASS(fr_amouse_key_parent_class)->create(o);
}

/* object api */
FRAction* fr_amouse_key_new(void) {
  return sys_object_new(FR_TYPE_AMOUSE_KEY, NULL);
}

FRAction *fr_amouse_key_new_I(void) {
  FRAction *o = fr_amouse_key_new();

  fr_amouse_key_create_i(o);

  return o;
}

static void fr_amouse_key_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_amouse_key_parent_class)->dispose(o);
}

static void fr_amouse_key_class_init(FRAMouseKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRActionClass *acls = FR_ACTION_CLASS(cls);

  ocls->dispose = fr_amouse_key_dispose;

  acls->create = fr_amouse_key_create_i;
  acls->check = fr_amouse_key_check_i;
}

static void fr_amouse_key_init(FRAMouseKey *self) {
  fr_action_set_name(FR_ACTION(self), "mouse_key");
}

FRAction* fr_amouse_key_get_static(void) {
  static FRAction *node = NULL;

  if(node != NULL) {
    return node;
  }

  node = fr_amouse_key_new_I();

  return node;
}
