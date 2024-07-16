#ifndef __FR_MEDIA_PIPELINE_H__
#define __FR_MEDIA_PIPELINE_H__

#include <Framework/Media/FrPipeline.h>

SYS_BEGIN_DECLS

#define FR_TYPE_MEDIA_PIPELINE (fr_media_pipeline_get_type())
#define FR_MEDIA_PIPELINE(o) ((FrMediaPipeline* )sys_object_cast_check(o, FR_TYPE_MEDIA_PIPELINE))
#define FR_MEDIA_PIPELINE_CLASS(o) ((FrMediaPipelineClass *)sys_class_cast_check(o, FR_TYPE_MEDIA_PIPELINE))
#define FR_MEDIA_PIPELINE_GET_CLASS(o) sys_instance_get_class(o, FrMediaPipelineClass)

struct _FrMediaPipelineClass {
  SysObjectClass parent;
};

struct _FrMediaPipeline {
  SysObject parent;

  /* <private> */
  FrDecoder* video_decoder;
  FrDecoder* audio_decoder;
  FrDecoder* subtitle_decoder;
  FrDecoder* packet_decoder;
  FrMediaPlayer* player;
  SysInt max_packet;
  SysInt min_packet;

  SysInt pkt_count;
  SysAsyncQueue image_queue;
  SysAsyncQueue sample_queue;
};

#define fr_media_pipeline_create(o) sys_object_create(o, FR_TYPE_MEDIA_PIPELINE)
SYS_API SysType fr_media_pipeline_get_type(void);
SYS_API FrMediaPipeline *fr_media_pipeline_new(void);

SYS_API void fr_media_pipeline_get_video_size(FrMediaPipeline *self,
    SysInt *width,
    SysInt *height);

SYS_API FrMediaFrame* fr_media_pipeline_get_image_frame(FrMediaPipeline* self);
SYS_API void fr_media_pipeline_wakeup_source(FrMediaPipeline *self);

SYS_API void fr_media_pipeline_run(FrMediaPipeline* self,
    FrMediaFile* file);

SYS_API FrMediaPipeline *fr_media_pipeline_new_I(void);

SYS_END_DECLS

#endif
