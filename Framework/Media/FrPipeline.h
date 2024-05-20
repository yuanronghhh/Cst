#ifndef __FR_PIPELINE_H__
#define __FR_PIPELINE_H__

#include <Framework/Media/FrMediaTask.h>

SYS_BEGIN_DECLS

#define FR_TYPE_PIPELINE (fr_pipeline_get_type())
#define FR_PIPELINE(o) ((FrPipeline* )sys_object_cast_check(o, FR_TYPE_PIPELINE))
#define FR_PIPELINE_CLASS(o) ((FrPipelineClass *)sys_class_cast_check(o, FR_TYPE_PIPELINE))
#define FR_PIPELINE_GET_CLASS(o) sys_instance_get_class(o, FrPipelineClass)

struct _FrPipelineClass {
  SysObjectClass parent;
};

struct _FrPipeline {
  SysObject parent;

  FrDecoder* video_decoder;
  FrDecoder* audio_decoder;
  FrDecoder* subtitle_decoder;
  FrDecoder* packet_decoder;

  SysAsyncQueue image_queue;
  SysAsyncQueue sample_queue;
  FrMediaTask task;
};

SYS_API SysType fr_pipeline_get_type(void);
SYS_API FrPipeline *fr_pipeline_new(void);
#define fr_pipeline_create(o) sys_object_create(o, FR_TYPE_PIPELINE)
void fr_pipeline_run(FrPipeline *self, FrMediaFile *file);

SYS_END_DECLS

#endif
