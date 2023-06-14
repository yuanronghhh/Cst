#include <Framework/DataType/FRMain.h>


struct _FRMainPrivate {
  SysBool is_running;
  SysList *sources;
  SysList *current;
  SysRecMutex mutex;
};

SYS_DEFINE_TYPE_WITH_PRIVATE(FRMain, fr_main, SYS_TYPE_OBJECT);

static FRMain *worker_loop = NULL;
static FRMain *main_loop = NULL;

/* worker thread */
static SysPointer main_work_func(SysPointer data) {
  sys_return_val_if_fail(worker_loop != NULL, NULL);

  FRSource *source = NULL;

  while(fr_main_is_running(main_loop)) {

    fr_main_iter_next(worker_loop, &source);

    sys_sleep(1e6);
    printf("%lld\t%p\n", sys_get_monoic_time(), sys_thread_self());
  }

  return NULL;
}

FRMain *fr_main_get_work_loop(void) {
  return worker_loop;
}

FRMain *fr_main_get_main_loop(void) {
  return main_loop;
}

void fr_main_lock(FRMain *self) {
  sys_return_if_fail(self != NULL);

  FRMainPrivate* priv = self->priv;

  sys_rec_mutex_lock(&priv->mutex);
}

void fr_main_unlock(FRMain *self) {
  sys_return_if_fail(self != NULL);

  FRMainPrivate* priv = self->priv;

  sys_rec_mutex_unlock(&priv->mutex);
}

void fr_main_stop(FRMain *self) {
  sys_return_if_fail(self != NULL);

  FRMainPrivate* priv = self->priv;

  priv->is_running = false;
}

void fr_main_attach(FRMain *self, FRSource *source) {
  sys_return_if_fail(self != NULL);

  fr_main_lock(self);

  FRMainPrivate* priv = self->priv;

  fr_source_set_main(source, self);
  priv->sources = sys_list_prepend(priv->sources, source);

  fr_main_unlock(self);
}

void fr_main_iter_next(FRMain *self, FRSource **source) {
  sys_return_if_fail(self != NULL);

  FRMainPrivate* priv = self->priv;
  SysList *next = NULL;

  fr_main_lock(self);

  if(priv->sources) {
    if(priv->current) {

      next = priv->current->next;
    }

    if(!next) {
      next = priv->sources;
    }

    priv->current = next;
    *source = next->data;

  } else {

    *source = NULL;
  }

  fr_main_unlock(self);
}

SysBool fr_main_is_running(FRMain *self) {
  sys_return_val_if_fail(self != NULL, false);

  FRMainPrivate* priv = self->priv;

  return (SysBool)sys_atomic_int_get(&priv->is_running);
}

static void fr_main_destroy(FRMain *self) {
  sys_return_if_fail(self != NULL);

  FRMainPrivate* priv = self->priv;

  sys_assert(priv->is_running == false);
  sys_list_free_full(priv->sources, (SysDestroyFunc)_sys_object_unref);
}

void fr_main_run(FRMain *self) {
  sys_return_if_fail(self != NULL);

  FRSource *source = NULL;

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

  fr_main_destroy(self);
}

void fr_main_setup(void) {
  SysThread *thread;

  main_loop = fr_main_new_I();
  worker_loop = fr_main_new_I();

  thread = sys_thread_new(main_work_func, worker_loop);

  UNUSED(thread);
}

void fr_main_teardown(void) {
  sys_object_unref(worker_loop);
}

/* object api */
static void fr_main_construct(SysObject *o) {
  FRMain* self = FR_MAIN(o);
  FRMainPrivate* priv = self->priv;

  priv->is_running = true;
  priv->sources = NULL;

  sys_rec_mutex_init(&priv->mutex);
}

FRMain* fr_main_new(void) {
  return sys_object_new(FR_TYPE_MAIN, NULL);
}

FRMain *fr_main_new_I(void) {
  FRMain *o = fr_main_new();

  fr_main_construct(SYS_OBJECT(o));

  return o;
}

static void fr_main_dispose(SysObject* o) {
  FRMain *self = FR_MAIN(o);
  FRMainPrivate* priv = self->priv;

  sys_rec_mutex_clear(&priv->mutex);

  SYS_OBJECT_CLASS(fr_main_parent_class)->dispose(o);
}

static void fr_main_class_init(FRMainClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)fr_main_construct;
  ocls->dispose = fr_main_dispose;
}

void fr_main_init(FRMain *self) {
  self->priv = fr_main_get_private(self);
}
