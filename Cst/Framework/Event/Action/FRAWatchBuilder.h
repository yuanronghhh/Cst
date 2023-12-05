#ifndef __FR_AWATCH_BUILDER_H__
#define __FR_AWATCH_BUILDER_H__

#include <Framework/Event/Action/FRAction.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_BUILDER (fr_awatch_builder_get_type())
#define FR_AWATCH_BUILDER(o) ((FRAWatchBuilder* )sys_object_cast_check(o, FR_TYPE_AWATCH_BUILDER))
#define FR_AWATCH_BUILDER_CLASS(o) ((FRAWatchBuilderClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_BUILDER))
#define FR_AWATCH_BUILDER_GET_CLASS(o) sys_instance_get_class(o, FRAWatchBuilderClass)

struct _FRAWatchBuilder {
  SysObject parent;

  /* <private> */
  SysChar *func_name;
  FREventFunc func;
  SysPointer user_data;

  // FRAction *action;
  SysType etype;
  SysInt event_enum;
  SysInt key;
  FRGetBoundFunc bound_func;
};

struct _FRAWatchBuilderClass {
  SysObjectClass parent;

  void  (*construct) (FRAWatchBuilder* self, const SysChar *func_name, FREventFunc func);
};

SYS_API SysType fr_awatch_builder_get_type(void);
SYS_API FRAWatchBuilder* fr_awatch_builder_new(void);
SYS_API FRAWatchBuilder *fr_awatch_builder_new_I(const SysChar *func_name, FREventFunc func);

SYS_API void fr_awatch_builder_build_awatch(FRAWatchBuilder *self, FRAWatch *o);
SYS_API void fr_awatch_builder_build_awatch_key(FRAWatchBuilder *self, FRAWatchKey *o);
SYS_API void fr_awatch_builder_build_awatch_any(FRAWatchBuilder *self, FRAWatchAny *o);
SYS_API void fr_awatch_builder_build_awatch_key_press(FRAWatchBuilder *self, FRAWatchKeyPress *o);
SYS_API void fr_awatch_builder_build_awatch_mouse_release(FRAWatchBuilder *self, FRAWatchMouseRelease *o);
SYS_API void fr_awatch_builder_build_awatch_mouse_press(FRAWatchBuilder *self, FRAWatchMousePress *o);
SYS_API void fr_awatch_builder_build_awatch_cursor_move(FRAWatchBuilder *self, FRAWatchCursorMove *o);

void fr_awatch_builder_set_key(FRAWatchBuilder *self, SysInt key);
SysInt fr_awatch_builder_get_key(FRAWatchBuilder *self);

void fr_awatch_builder_set_etype(FRAWatchBuilder *self, SysType etype);
SysType fr_awatch_builder_get_etype(FRAWatchBuilder *self);

void fr_awatch_builder_set_event_enum(FRAWatchBuilder *self, SysInt event_enum);
SysInt fr_awatch_builder_get_event_enum(FRAWatchBuilder *self);

SYS_END_DECLS

#endif

