#ifndef __FR_DECODER_H__
#define __FR_DECODER_H__

#include <Framework/DataType/FrJob.h>

SYS_BEGIN_DECLS

#define FR_TYPE_DECODER (fr_decoder_get_type())
#define FR_DECODER(o) ((FrDecoder* )sys_object_cast_check(o, FR_TYPE_DECODER))
#define FR_DECODER_CLASS(o) ((FrDecoderClass *)sys_class_cast_check(o, FR_TYPE_DECODER))
#define FR_DECODER_GET_CLASS(o) sys_instance_get_class(o, FrDecoderClass)

struct _FrDecoderContext {
  const SysChar *name;
};

struct _FrDecoderClass {
  SysObjectClass parent;

  void (*construct) (FrDecoder* o, FrDecoderContext *info);
  SysInt (*decode) (FrDecoder *o, FrPacket **npkt);
  SysInt (*open) (FrDecoder* o);
  SysInt (*close) (FrDecoder* o);
};

struct _FrDecoder {
  SysObject parent;
  // FrJob job;

  /* <private> */
  SysChar* name;

  /* current codec */
  SysInt64 start_pts;
  SysInt serial;
  SysInt64 max_pkt;
  SysInt64 min_pkt;
  SysBool eof;
  SysPointer user_data;
};


SYS_API SysType fr_decoder_get_type(void);
SYS_API FrDecoder *fr_decoder_new(void);

SYS_API void fr_decoder_setup(void);
SYS_API void fr_decoder_teardown(void);

SYS_API const SysChar * fr_decoder_get_name(FrDecoder *self);
SYS_API SysInt fr_decoder_open(FrDecoder* self);
SYS_API SysInt fr_decoder_close(FrDecoder* self);

SYS_API SysBool fr_decoder_start(FrDecoder* self);
SYS_API void fr_decoder_stop(FrDecoder* self);

SYS_API void fr_decoder_run_async(FrDecoder *self,
    FrTaskFunc func, 
    SysPointer user_data);

void fr_decoder_set_eof(FrDecoder* self, SysBool eof);

SYS_API void fr_decoder_set_user_data(FrDecoder *self, SysPointer user_data);
SYS_API SysPointer fr_decoder_get_user_data(FrDecoder *self);

void fr_decoder_set_eof(FrDecoder *self, SysBool eof);
SysBool fr_decoder_get_eof(FrDecoder *self);

SYS_END_DECLS

#endif
