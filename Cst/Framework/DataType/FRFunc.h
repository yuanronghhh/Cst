#ifndef __FR_FUNC__
#define __FR_FUNC__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define FR_FUNC_DEFINE_EVENT(func) SysInt func(FREvent *e, SysPointer user_data)
#define FR_FUNC_DEFINE_ANY(func) func
#define FR_FUNC_PREFIX "F_:"
#define FR_FUNC_EVENT_PREFIX "F_EVENT:"
#define FR_FUNC_EVENT(func) "F_EVENT:"#func, (SysFunc)func
#define FR_FUNC_ANY(func) "F_ANY:"#func, func

SYS_END_DECLS

#endif
