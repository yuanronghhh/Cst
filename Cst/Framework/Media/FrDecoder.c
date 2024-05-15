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

void fr_decoder_wait(FrDecoder *self) {
  sys_async_queue_lock(&self->queue);
  sys_cond_wait(&self->queue.cond, &self->queue.mutex);
  sys_async_queue_unlock(&self->queue);
}

void fr_decoder_wakeup(FrDecoder *self) {
  sys_async_queue_lock(&self->queue);
  sys_cond_signal(&self->queue.cond);
  sys_async_queue_unlock(&self->queue);
}

static SysPointer decoder_thread(SysPointer user_data) {
  FrDecoder *self = user_data;
  SysInt err;

  while(self->running) {
    err = fr_decoder_decode_it(self);
    if(err >= 0 || err == FR_MEDIA_STATE_EAGAIN) {
      continue;
    }

    if(err == FR_MEDIA_STATE_EOF) {
      sys_debug_N("\"%s\" stop when occur packet eof", self->name);
      break;
    } else {

      sys_error_N("decoder stoped: %s", av_err2str(err));
      break;
    }
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
  self->thread = thread;

  return 0;
}

void fr_decoder_stop(FrDecoder* self) {
  self->running = false;

  fr_decoder_wakeup(self);
  sys_thread_join(self->thread);
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
  if(npkt == NULL) { return false; }
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
}
