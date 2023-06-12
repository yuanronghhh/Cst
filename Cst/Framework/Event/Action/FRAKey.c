#include <Framework/Event/Action/FRAKey.h>
#include <Framework/Event/Base/FREventKey.h>


struct _FRAKeyPrivate {
  SysChar reserved;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRAKey, fr_akey, FR_TYPE_ACTION);

SysBool fr_akey_check_i (FRAction *self, FREvent *e) {
  if(!fr_event_is(e, FR_TYPE_EVENT_KEY)) {
    return false;
  }

  return true;
}

void fr_akey_dispatch_i (FRAction *o, FREvent *e) {

  FR_ACTION_CLASS(fr_akey_parent_class)->dispatch(o, e);
}

/* object api */
static void fr_akey_create_i(FRAction *o) {

  FR_ACTION_CLASS(fr_akey_parent_class)->create(o);
}

FRAction* fr_akey_new(void) {
  return sys_object_new(FR_TYPE_AKEY, NULL);
}

static void fr_akey_dispose(SysObject* o) {

  SYS_OBJECT_CLASS(fr_akey_parent_class)->dispose(o);
}

static void fr_akey_class_init(FRAKeyClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  FRActionClass *acls = FR_ACTION_CLASS(cls);

  acls->create = fr_akey_create_i;
  acls->check = fr_akey_check_i;
  acls->dispatch = fr_akey_dispatch_i;

  ocls->dispose = fr_akey_dispose;
}

FRAction *fr_akey_new_I(void) {
  FRAction *o = fr_akey_new();

  fr_akey_create_i(o);

  return o;
}

void fr_akey_init(FRAKey *self) {
  self->priv = fr_akey_get_private(self);

  fr_action_set_name(FR_ACTION(self), "key");
}

FRAction* fr_akey_get_static(void) {
  static FRAction *node = NULL;

  if(node != NULL) {
    return node;
  }

  node = fr_akey_new_I();

  return node;
}
