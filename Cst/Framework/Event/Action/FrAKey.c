#include <Framework/Event/Action/FrAKey.h>
#include <Framework/Event/Base/FrEventKey.h>


SYS_DEFINE_TYPE(FrAKey, fr_akey, FR_TYPE_ACTION);

SysBool fr_akey_check_i (FrAction *self, FrEvent *e) {
  if(!fr_event_is(e, FR_TYPE_EVENT_KEY)) {
    return false;
  }

  return true;
}

void fr_akey_dispatch_i (FrAction *o, FrEvent *e) {

  FR_ACTION_CLASS(fr_akey_parent_class)->dispatch(o, e);
}

/* object api */
static void fr_akey_create_i(FrAction *o) {

  FR_ACTION_CLASS(fr_akey_parent_class)->create(o);
}

FrAction* fr_akey_new(void) {
  return sys_object_new(FR_TYPE_AKEY, NULL);
}

static void fr_akey_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_akey_parent_class)->dispose(o);
}

static void fr_akey_class_init(FrAKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FrActionClass *acls = FR_ACTION_CLASS(cls);

  acls->create = fr_akey_create_i;
  acls->check = fr_akey_check_i;
  acls->dispatch = fr_akey_dispatch_i;

  ocls->dispose = fr_akey_dispose;
}

FrAction *fr_akey_new_I(void) {
  FrAction *o = fr_akey_new();

  fr_akey_create_i(o);

  return o;
}

void fr_akey_init(FrAKey *self) {

  fr_action_set_name(FR_ACTION(self), "key");
}

FrAction* fr_akey_get_static(void) {
  static FrAction *node = NULL;

  if(node != NULL) {
    return node;
  }

  node = fr_akey_new_I();

  return node;
}
