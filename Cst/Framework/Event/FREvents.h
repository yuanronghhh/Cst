#ifndef __FR_EVENTS_H__
#define __FR_EVENTS_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

SYS_API void fr_events_push_head(FREvent *e);
SYS_API SysBool fr_events_check(void);
SYS_API SysList* fr_events_prepend_action(FRAction* action);
SYS_API FREvent *fr_events_get(void);
SYS_API void fr_events_dispatch(FREvent *e);

SYS_API void fr_events_setup(void);
SYS_API void fr_events_teardown(void);

SYS_END_DECLS

#endif

