#ifndef __CST_CONTEXT_H__
#define __CST_CONTEXT_H__

#include <CstCore/CstConfig.h>
#include <CstCore/Driver/CstTypes.h>

SYS_BEGIN_DECLS

SYS_API CstContext* cst_context_get (void);
SYS_API void cst_context_setup (SysInt argc, const SysChar* argv[]);
SYS_API void cst_context_teardown (void);
SYS_API const SysChar* cst_context_progname(CstContext *c);

SYS_END_DECLS

#endif

