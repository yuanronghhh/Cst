#ifndef __FR_AWATCH_BUILDER_H__
#define __FR_AWATCH_BUILDER_H__

#include <Framework/Event/Action/FrAction.h>

SYS_BEGIN_DECLS

#define FR_TYPE_AWATCH_BUILDER (fr_awatch_builder_get_type())
#define FR_AWATCH_BUILDER(o) ((FrAWatchBuilder* )sys_object_cast_check(o, FR_TYPE_AWATCH_BUILDER))
#define FR_AWATCH_BUILDER_CLASS(o) ((FrAWatchBuilderClass *)sys_class_cast_check(o, FR_TYPE_AWATCH_BUILDER))
#define FR_AWATCH_BUILDER_GET_CLASS(o) sys_instance_get_class(o, FrAWatchBuilderClass)

struct _FrAWatchBuilder {
  SysObject parent;

  /* <private> */
  SysChar *func_name;
  FrEventFunc func;
  SysPointer user_data;

  // FrAction *action;
  SysType etype;
  SysInt event_enum;
  SysInt key;
  FrGetBoundFunc bound_func;
};

struct _FrAWatchBuilderClass {
  SysObjectClass parent;

  void  (*construct) (FrAWatchBuilder* self, const SysChar *func_name, FrEventFunc func);
};

SYS_API SysType fr_awatch_builder_get_type(void);
SYS_API FrAWatchBuilder* fr_awatch_builder_new(void);
SYS_API FrAWatchBuilder *fr_awatch_builder_new_I(const SysChar *func_name, FrEventFunc func);

SYS_API void fr_awatch_builder_build_awatch(FrAWatchBuilder *self, FrAWatch *o);
SYS_API void fr_awatch_builder_build_awatch_key(FrAWatchBuilder *self, FrAWatchKey *o);
SYS_API void fr_awatch_builder_build_awatch_any(FrAWatchBuilder *self, FrAWatchAny *o);
SYS_API void fr_awatch_builder_build_awatch_key_press(FrAWatchBuilder *self, FrAWatchKeyPress *o);
SYS_API void fr_awatch_builder_build_awatch_mouse_release(FrAWatchBuilder *self, FrAWatchMouseRelease *o);
SYS_API void fr_awatch_builder_build_awatch_mouse_press(FrAWatchBuilder *self, FrAWatchMousePress *o);
SYS_API void fr_awatch_builder_build_awatch_cursor_move(FrAWatchBuilder *self, FrAWatchCursorMove *o);

void fr_awatch_builder_set_key(FrAWatchBuilder *self, SysInt key);
SysInt fr_awatch_builder_get_key(FrAWatchBuilder *self);

void fr_awatch_builder_set_etype(FrAWatchBuilder *self, SysType etype);
SysType fr_awatch_builder_get_etype(FrAWatchBuilder *self);

void fr_awatch_builder_set_event_enum(FrAWatchBuilder *self, SysInt event_enum);
SysInt fr_awatch_builder_get_event_enum(FrAWatchBuilder *self);

SYS_END_DECLS

#endif

