#ifndef __FR_CANVAS_H__
#define __FR_CANVAS_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

void fr_canvas_setup(void);
void fr_canvas_teardown(void);

SYS_API SysInt fr_canvas_rounded_rectangle(FRContext * context, SysDouble x, SysDouble y, SysDouble w, SysDouble h, SysDouble radius);

SYS_END_DECLS

#endif

