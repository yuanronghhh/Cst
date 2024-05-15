#include <Framework/Media/FrDecoder.h>
#include <Framework/Media/FrPacket.h>

SYS_DEFINE_TYPE(FrDecoder, fr_decoder, SYS_TYPE_OBJECT);

const SysChar* fr_decoder_get_name(FrDecoder* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
}

static SysInt fr_decoder_decode_it_i(FrDecoder *self, SysPointer user_data) {
  FrPacket *pkt = NULL;

  if(fr_decoder_pop_packet(self, &pkt)) {
    return 0;
  }

  return FR_MEDIA_STATE_EAGAIN;
}

void fr_decoder_lock(FrDecoder* self) {
  sys_async_queue_lock(&self->queue);
}

void fr_decoder_unlock(FrDecoder* self) {
  sys_async_queue_unlock(&self->queue);
}

void fr_thread_worker_wait(FrThreadWorker *self) {
  sys_mutex_lock(self->mutex);
  if (self->state != self->cmd) {

    sys_cond_wait(self->cond, self->mutex);
  }
  sys_mutex_unlock(self->mutex);
}

void fr_thread_worker_create(FrThreadWorker* self, 
  SysCond *cond, 
  SysMutex *mutex) {

  self->cmd = -2;
  self->state = -1;
  self->cond = cond;
  self->mutex = mutex;
  self->thread = NULL;
  self->wait = true;
}

void fr_thread_worker_set_thread(FrThreadWorker* self, SysThread *thread) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(thread != NULL);

  self->thread = thread;
}

void fr_thread_worker_join(FrThreadWorker* self) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(self->thread != NULL);

  sys_thread_join(self->thread);
}

void fr_thread_worker_cmd(FrThreadWorker * self, SysInt cmd) {
  sys_mutex_lock(self->mutex);
  self->cmd = cmd;

  if (self->state != self->cmd) {

    sys_cond_wait(self->cond, self->mutex);
  }
  sys_mutex_unlock(self->mutex);
}

void fr_thread_worker_set_func(FrThreadWorker* self, FrThreadWorkerFunc func) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(func != NULL);

  self->func = func;
}

SysInt fr_thread_worker_trigger(FrThreadWorker* self, SysPointer user_data) {
  sys_return_val_if_fail(self != NULL, false);

  SysInt err = self->func(self, self->cmd, self->data, user_data);

  self->state = self->cmd;
  if(self->wait) {

    sys_cond_signal(self->cond);
  }
  self->cmd = -2;
  self->state = -1;

  return err;
}

SysInt decoder_handle_state(FrThreadWorker* self, 
  SysInt cmd,
  SysPointer data,
  SysPointer user_data) {
  FrDecoder* decoder = user_data;

  switch (cmd) {
    case FR_DECODER_CMD_INIT:
      decoder->inited = true;
      return 1;
    case FR_DECODER_CMD_STOP:
      sys_async_queue_clear(&decoder->queue);
      decoder->running = false;
      return -1;
    default:
      break;
  }

  return 0;
}

static SysPointer decoder_thread(SysPointer user_data) {
  FrDecoder *self = user_data;
  SysInt err;

  fr_thread_worker_set_func(&self->thread_woker, decoder_handle_state);

  while(self->running) {
    if(fr_thread_worker_trigger(&self->thread_woker, self) < 0) {
      break;
    }

    err = fr_decoder_decode_it(self);
    if(err >= 0 || err == FR_MEDIA_STATE_EAGAIN || err == FR_MEDIA_STATE_EOF) {
      continue;
    }

    sys_error_N("decoder stoped: %s", av_err2str(err));
    break;
  }

  return NULL;
}

SysInt fr_decoder_start(FrDecoder* self) {
  SysThread *thread;
  const SysChar *name = self->name;

  thread = sys_thread_new(name, decoder_thread, self);
  if (thread == NULL) {
    sys_error_N("decoder start failed: \"%s\"", name);
    return -1;
  }
  fr_thread_worker_set_thread(&self->thread_woker, thread);
  fr_thread_worker_cmd(&self->thread_woker, FR_DECODER_CMD_INIT);

  return 0;
}

static void wakeup_queue(FrDecoder* self) {
  FrPacket* pkt;

  sys_async_queue_lock(&self->queue);

  pkt = fr_packet_new();
  fr_packet_set_serial(pkt, -1);
  sys_async_queue_push_unlocked(&self->queue, pkt);

  sys_async_queue_unlock(&self->queue);
}

void fr_decoder_stop(FrDecoder* self) {
  wakeup_queue(self);
  fr_thread_worker_cmd(&self->thread_woker, FR_DECODER_CMD_STOP);
  fr_thread_worker_join(&self->thread_woker);

  sys_debug_N("decoder dispose: %s", self->name);
}

void fr_decoder_set_running(FrDecoder *self, SysBool running) {
  sys_return_if_fail(self != NULL);

  self->running = running;
}

SysBool fr_decoder_get_running(FrDecoder *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->running;
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

  sys_async_queue_lock(&self->queue);

  npkt = sys_async_queue_pop_unlocked(&self->queue);
  if(fr_packet_empty(npkt)) { 
    sys_object_unref(npkt);

    return false; 
  }
  self->serial = fr_packet_get_serial(npkt);
  *pkt = npkt;

  sys_async_queue_unlock(&self->queue);

  return true;
}

SysBool fr_decoder_push_packet(FrDecoder* self, FrPacket *pkt) {
  sys_return_val_if_fail(self != NULL, false);
  sys_return_val_if_fail(pkt != NULL, false);

  sys_async_queue_lock(&self->queue);

  sys_async_queue_push_unlocked(&self->queue, pkt);

  sys_async_queue_unlock(&self->queue);

  return true;
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

  sys_assert(self->queue.queue.length == 0);
  sys_async_queue_clear(&self->queue);
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

  sys_async_queue_init_full(&self->queue, (SysDestroyFunc)_sys_object_unref);

  fr_thread_worker_create(&self->thread_woker,
      &self->queue.cond, 
      &self->queue.mutex);
}
