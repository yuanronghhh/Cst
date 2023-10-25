#ifndef __FR_AWATCH_BUILDER_H__
#define __FR_AWATCH_BUILDER_H__

#include <Framework/Event/Action/FRAction.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_BUILDER (fr_awatch_builder_get_type())
#define FR_AWATCH_BUILDER(o) ((FRAWatchBuilder* )sys_object_cast_check(o, FR_TYPE_AWATCH_BUILDER))
#define FR_AWATCH_BUILDER_CLASS(o) ((FRAWatchBuilderClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_BUILDER))
#define FR_AWATCH_BUILDER_GET_CLASS(o) sys_instance_get_class(o, FRAWatchBuilderClass)

struct _FRAWatchBuilderProps {
};

struct _FRAWatchBuilder {
  SysObject parent;

  /* <private> */
  FRAction *action;
  SysType etype;
  SysInt key;
  FRGetBoundFunc get_bound_func;
  SysPointer app_data;
};

struct _FRAWatchBuilderClass {
  SysObjectClass parent;

  void  (*construct) (FRAWatchBuilder* self, const SysChar *func_name, FREventFunc func);
};

SYS_API SysType fr_awatch_builder_get_type(void);
SYS_API FRAWatchBuilder* fr_awatch_builder_new(void);
SYS_API FRAWatchBuilder *fr_awatch_builder_new_I(const SysChar *func_name, FREventFunc func);

SYS_END_DECLS

#endif

