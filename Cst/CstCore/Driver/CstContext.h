#ifndef __CST_CONTEXT_H__
#define __CST_CONTEXT_H__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

void cst_context_setup (void);
void cst_context_teardown(void);

void cst_context_set_progname(CstContext *self, const SysChar * progname);
const SysChar * cst_context_get_progname(CstContext *self);

SYS_END_DECLS

#endif
