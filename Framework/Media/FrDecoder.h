#ifndef __FR_DECODER_H__
#define __FR_DECODER_H__

#include <Framework/Media/FrMediaTask.h>

SYS_BEGIN_DECLS

#define FR_TYPE_DECODER (fr_decoder_get_type())
#define FR_DECODER(o) ((FrDecoder* )sys_object_cast_check(o, FR_TYPE_DECODER))
#define FR_DECODER_CLASS(o) ((FrDecoderClass *)sys_class_cast_check(o, FR_TYPE_DECODER))
#define FR_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrDecoderClass)

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
  SysUInt limit;

  struct {
    SysQueue queue;
    SysCond cond;
    SysMutex mutex;
  } ctrl;

  FrMediaTask *task;
  FR_MEDIA_STATE_ENUM state;
  SysThread* thread;

  SysPointer user_data;
};


SYS_API SysType fr_decoder_get_type(void);
SYS_API FrDecoder *fr_decoder_new(void);

SYS_API const SysChar * fr_decoder_get_name(FrDecoder *self);
SYS_API SysInt fr_decoder_decode_it(FrDecoder* self);
SYS_API SysInt fr_decoder_open(FrDecoder* self);
SYS_API SysInt fr_decoder_close(FrDecoder* self);

SysBool fr_decoder_push_packet_unlock(FrDecoder* self, FrPacket *pkt);
SysBool fr_decoder_pop_packet_unlock(FrDecoder* self,
    FrPacket **pkt);

SysBool fr_decoder_push_packet(FrDecoder* self, FrPacket* pkt);
SysBool fr_decoder_pop_packet(FrDecoder* self,
    FrPacket** pkt);

SysInt fr_decoder_start(FrDecoder* self);
void fr_decoder_stop(FrDecoder* self);
void fr_decoder_wakeup_unlock(FrDecoder* self);
void fr_decoder_wait (FrDecoder* self);
void fr_decoder_set_task(FrDecoder *self, FrMediaTask *task);
SysBool fr_decoder_need_wait(FrDecoder* self);

void fr_decoder_set_state(FrDecoder *self, FR_MEDIA_STATE_ENUM state);
FR_MEDIA_STATE_ENUM fr_decoder_get_state(FrDecoder *self);

void fr_decoder_set_user_data(FrDecoder *self, SysPointer user_data);
SysPointer fr_decoder_get_user_data(FrDecoder *self);

SYS_END_DECLS

#endif
