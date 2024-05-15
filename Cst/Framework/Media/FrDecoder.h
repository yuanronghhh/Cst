#ifndef __FR_DECODER_H__
#define __FR_DECODER_H__

#include <Framework/Media/FrMediaCommon.h>

SYS_BEGIN_DECLS

#define FR_TYPE_DECODER (fr_decoder_get_type())
#define FR_DECODER(o) ((FrDecoder* )sys_object_cast_check(o, FR_TYPE_DECODER))
#define FR_DECODER_CLASS(o) ((FrDecoderClass *)sys_class_cast_check(o, FR_TYPE_DECODER))
#define FR_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrDecoderClass)

typedef SysInt (*FrThreadWorkerFunc) (FrThreadWorker* self,
    SysInt cmd, 
    SysPointer data,
    SysPointer user_data);

struct _FrThreadWorker {
  SysInt cmd;
  SysInt state;
  SysBool wait;
  SysPointer data;
  SysMutex* mutex;
  SysCond* cond;
  SysThread* thread;
  FrThreadWorkerFunc func;
};

struct _FrDecoderClass {
  SysObjectClass parent;

  void (*construct) (
    FrDecoder* o,
    const SysChar *name);

  SysInt (*decode_it) (FrDecoder* o, SysPointer user_data);
  SysInt (*open) (FrDecoder* o);
  SysInt (*close) (FrDecoder* o);
};

struct _FrDecoder {
  SysObject parent;

  /* <private> */
  SysChar* name;
  FR_MEDIA_ENUM decoder_type;

  /* current codec */
  SysInt64 start_pts;
  SysInt serial;

  SysAsyncQueue queue;
  SysBool running;
  SysBool inited;
  FrThreadWorker thread_woker;

  SysPointer user_data;
};


SYS_API SysType fr_decoder_get_type(void);
SYS_API FrDecoder *fr_decoder_new(void);

SYS_API const SysChar * fr_decoder_get_name(FrDecoder *self);
SYS_API SysInt fr_decoder_decode_it(FrDecoder* self);
SYS_API SysInt fr_decoder_open(FrDecoder* self);
SYS_API SysInt fr_decoder_close(FrDecoder* self);
SYS_API void fr_decoder_unlock(FrDecoder* self);
SYS_API void fr_decoder_lock(FrDecoder* self);

SysBool fr_decoder_push_packet(FrDecoder* self, FrPacket *pkt);
SysBool fr_decoder_pop_packet(FrDecoder* self,
    FrPacket **pkt);

SysInt fr_decoder_start(FrDecoder* self);
void fr_decoder_stop(FrDecoder* self);

void fr_decoder_set_running(FrDecoder *self, SysBool running);
SysBool fr_decoder_get_running(FrDecoder *self);

void fr_decoder_set_user_data(FrDecoder *self, SysPointer user_data);
SysPointer fr_decoder_get_user_data(FrDecoder *self);

SYS_END_DECLS

#endif
