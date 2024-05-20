#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrPacket.h>

SYS_DEFINE_TYPE(FrDecoder, fr_decoder, SYS_TYPE_OBJECT);

const SysChar* fr_decoder_get_name(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

static SysInt fr_decoder_decode_it_i(FrDecoder *self, SysPointer user_data) {
  sys_return_val_if_fail(self != NULL, -1);

  return FR_MEDIA_STATE_EAGAIN;
}

void fr_decoder_set_task(FrDecoder* self, FrMediaTask* task) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(task != NULL);

  sys_mutex_lock(&self->ctrl.mutex);
  sys_assert(self->task == NULL);

  self->task = task;
  sys_cond_signal(&self->ctrl.cond);

  sys_mutex_unlock(&self->ctrl.mutex);

}

static SysInt decoder_process_task(FrDecoder *self) {
  SysInt result;

  sys_mutex_lock(&self->ctrl.mutex);

  fr_media_task_run(self->task);

  result = POINTER_TO_INT(fr_media_task_result(self->task));
  self->task = NULL;

  sys_mutex_unlock(&self->ctrl.mutex);

  return result;
}

static SysInt decoder_process_packet(FrDecoder* self) {
  SysInt err;

  sys_mutex_lock(&self->ctrl.mutex);

  if (!sys_queue_get_length(&self->ctrl.queue)) {
    return FR_MEDIA_STATE_EAGAIN;
  }

  err = fr_decoder_decode_it(self);
  if (err >= 0 || err == FR_MEDIA_STATE_EAGAIN) {
    return 0;
  }
  sys_error_N("decoder stoped: %s", av_err2str(err));

  sys_mutex_unlock(&self->ctrl.mutex);

  return err;
}

static SysPointer decoder_thread(SysPointer user_data) {
  FrDecoder *self = user_data;
  SysInt err = 0;

  while (self->running) {
    if (self->task) {

      err = decoder_process_task(self);
    } else {

      err = decoder_process_packet(self);
    }

    if (err >= 0) {

      fr_decoder_wait(self);
    }
  }

  return NULL;
}

void fr_decoder_wait (FrDecoder* self) {
  sys_mutex_lock(&self->ctrl.mutex);
  sys_cond_wait(&self->ctrl.cond, &self->ctrl.mutex);
  sys_debug_N("wakeup %s", self->name);
  sys_mutex_unlock(&self->ctrl.mutex);
}

static SysPointer init_it(FrMediaTask* task, SysPointer user_data) {
  FrDecoder* self = user_data;
  sys_assert(self->inited == false);

  sys_debug_N("init %s", self->name);
  self->inited = true;
  return NULL;
}

void decoder_wait_init(FrDecoder* self) {
  FrMediaTask task = { 0 };
  task.handler = init_it;
  task.data = self;

  fr_decoder_set_task(self, &task);
  fr_media_task_wait(&task);
}

SysInt fr_decoder_start(FrDecoder* self) {
  SysThread *thread;
  const SysChar *name = self->name;

  thread = sys_thread_new(name, decoder_thread, self);
  if (thread == NULL) {
    sys_error_N("decoder start failed: \"%s\"", name);
    return -1;
  }
  self->thread = thread;
  decoder_wait_init(self);

  return 0;
}

static SysPointer stop_it(FrMediaTask *task, SysPointer user_data) {
  FrDecoder *self = user_data;

  sys_debug_N("stop it: %s", self->name);
  self->running = false;
  return INT_TO_POINTER(-1);
}

void fr_decoder_stop(FrDecoder* self) {
  sys_return_if_fail(self != NULL);

  FrMediaTask task = {0};
  task.handler = stop_it;
  task.data = self;

  fr_decoder_set_task(self, &task);
  fr_media_task_wait(&task);
}

void fr_decoder_set_running(FrDecoder *self, SysBool running) {
  sys_return_if_fail(self != NULL);
  sys_mutex_lock(&self->ctrl.mutex);

  self->running = running;

  sys_mutex_unlock(&self->ctrl.mutex);
}

SysBool fr_decoder_get_running(FrDecoder *self) {
  sys_return_val_if_fail(self != NULL, false);
  SysInt r;
  
  sys_mutex_lock(&self->ctrl.mutex);
  r = self->running;
  sys_mutex_unlock(&self->ctrl.mutex);

  return r;
}

void fr_decoder_set_user_data(FrDecoder *self, SysPointer user_data) {
  sys_return_if_fail(self != NULL);

  self->user_data = user_data;
}

SysPointer fr_decoder_get_user_data(FrDecoder *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->user_data;
}

SysBool fr_decoder_pop_packet(FrDecoder* self,
    FrPacket **pkt) {
  sys_return_val_if_fail(self != NULL, -1);
  sys_return_val_if_fail(*pkt == NULL, -1);
  FrPacket *npkt;

  npkt = sys_queue_pop_tail(&self->ctrl.queue);
  if(fr_packet_empty(npkt)) {
    sys_object_unref(npkt);

    return false;
  }
  self->serial = fr_packet_get_serial(npkt);
  *pkt = npkt;

  return true;
}

SysBool fr_decoder_push_packet(FrDecoder* self, FrPacket *pkt) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(pkt != NULL, false);
  SysBool r;

  sys_mutex_lock(&self->ctrl.mutex);

  if(sys_queue_get_length(&self->ctrl.queue) < self->limit) {

    sys_queue_push_head (&self->ctrl.queue, pkt);
    r = true;

  } else {

    r = false;
  }

  sys_mutex_unlock(&self->ctrl.mutex);

  return r;
}

SysInt fr_decoder_open(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, -1);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->open, -1);

  return cls->open(self);
}

SysInt fr_decoder_decode_it(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, -1);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->decode_it, -1);

  return cls->decode_it(self, self->user_data);
}

SysInt fr_decoder_close(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, -1);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);
  sys_return_val_if_fail(cls->close, -1);

  return cls->close(self);
}

void fr_decoder_construct(FrDecoder* self,
    const SysChar *name) {
  sys_return_if_fail(self != NULL);

  FrDecoderClass* cls = FR_DECODER_GET_CLASS(self);

  sys_return_if_fail(cls->construct);

  cls->construct(self, name);
}

SysInt fr_decoder_open_i(FrDecoder* o) {
  return 0;
}

SysInt fr_decoder_close_i(FrDecoder* o) {
  return 0;
}

/* object api */
static void fr_decoder_construct_i(FrDecoder *self,
    const SysChar *name) {
  self->name = sys_strdup(name);
}

FrDecoder* fr_decoder_new(void) {
  return sys_object_new(FR_TYPE_DECODER, NULL);
}

static void fr_decoder_dispose(SysObject* o) {
  FrDecoder *self = FR_DECODER(o);

  fr_decoder_stop(self);
  sys_thread_join(self->thread);
  sys_thread_unref(self->thread);

  sys_queue_clear(&self->ctrl.queue);
  sys_free_N(self->name);


  SYS_OBJECT_CLASS(fr_decoder_parent_class)->dispose(o);
}

static void fr_decoder_class_init(FrDecoderClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  cls->construct = fr_decoder_construct_i;
  cls->open = fr_decoder_open_i;
  cls->close = fr_decoder_close_i;
  cls->decode_it = fr_decoder_decode_it_i;

  ocls->dispose = fr_decoder_dispose;
}

void fr_decoder_init(FrDecoder* self) {
  self->start_pts = -1;
  self->serial = -1;
  self->running = true;
  self->limit = 20;
  self->task = NULL;

  sys_queue_init(&self->ctrl.queue);
}
