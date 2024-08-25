#include <Framework/DataType/FrMain.h>

struct _FrMainPrivate {
  SysBool is_running;
  SysList *sources;
  SysList *current;
  SysRecMutex mutex;
};

SYS_DEFINE_TYPE(FrMain, fr_main, SYS_TYPE_OBJECT);

static FrMain *main_loop = NULL;

FrMain *fr_main_get_main_loop(void) {
  return main_loop;
}

void fr_main_lock(FrMain *self) {
  sys_return_if_fail(self != NULL);

  sys_rec_mutex_lock(&self->mutex);
}

void fr_main_unlock(FrMain *self) {
  sys_return_if_fail(self != NULL);

  sys_rec_mutex_unlock(&self->mutex);
}

void fr_main_stop(FrMain *self) {
  sys_return_if_fail(self != NULL);

  fr_main_lock(self);

  self->is_running = false;

  fr_main_unlock(self);
}

void fr_main_attach(FrMain *self, FrSource *source) {
  sys_return_if_fail(self != NULL);

  fr_main_lock(self);

  fr_source_set_main(source, self);
  self->sources = sys_list_prepend(self->sources, source);

  fr_main_unlock(self);
}

void fr_main_iter_next(FrMain *self, FrSource **source) {
  sys_return_if_fail(self != NULL);

  SysList *next = NULL;

  fr_main_lock(self);

  if(self->sources) {
    if(self->current) {

      next = self->current->next;
    }

    if(!next) {
      next = self->sources;
    }

    self->current = next;
    *source = next->data;

  } else {

    *source = NULL;
  }

  fr_main_unlock(self);
}

SysBool fr_main_is_running(FrMain *self) {
  sys_return_val_if_fail(self != NULL, false);

  return (SysBool)sys_atomic_int_get(&self->is_running);
}

static void fr_main_clear(FrMain *self) {
  sys_return_if_fail(self != NULL);

  sys_assert(self->is_running == false);

  sys_list_free_full(self->sources, (SysDestroyFunc)_sys_object_unref);
}

void fr_main_run(FrMain *self) {
  sys_return_if_fail(self != NULL);

  FrSource *source = NULL;

  while (fr_main_is_running(self)) {
    fr_main_iter_next(self, &source);

    if(source == NULL) {
      fr_main_stop(self);
      continue;
    }

    if (!fr_source_check(source)) {
      continue;
    }

    fr_source_prepare(source);

    fr_source_dispatch(source);

    fr_source_finish(source);
  }

  fr_main_clear(self);
}

void fr_main_setup(void) {
  main_loop = fr_main_new_I();
}

void fr_main_teardown(void) {
  sys_assert(main_loop != NULL && "worker should not be used before main teardown.");
  fr_main_stop(main_loop);
  sys_object_unref(main_loop);
}

/* object api */
static void fr_main_construct(FrMain *self) {
  self->is_running = true;
  self->sources = NULL;

  sys_rec_mutex_init(&self->mutex);
}

FrMain* fr_main_new(void) {
  return sys_object_new(FR_TYPE_MAIN, NULL);
}

FrMain *fr_main_new_I(void) {
  FrMain *o = fr_main_new();

  fr_main_construct(o);

  return o;
}

static void fr_main_dispose(SysObject* o) {
  FrMain *self = FR_MAIN(o);

  sys_rec_mutex_clear(&self->mutex);
}

static void fr_main_class_init(FrMainClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = fr_main_dispose;
}

void fr_main_init(FrMain *self) {
}
