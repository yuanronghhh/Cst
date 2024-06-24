#ifndef __FR_PIPELINE_H__
#define __FR_PIPELINE_H__

#include <Framework/DataType/FrTask.h>

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
};

SYS_API SysType fr_pipeline_get_type(void);
SYS_API FrPipeline *fr_pipeline_new(void);

SYS_END_DECLS

#endif
