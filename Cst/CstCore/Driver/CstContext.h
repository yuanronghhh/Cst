#ifndef __CST_CONTEXT_H__
#define __CST_CONTEXT_H__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

CstContext* cst_context_new (void);
void cst_context_free(CstContext* self);

void cst_context_set_args(CstContext *self, int argc, char* argv[]);

void cst_context_set_progname(CstContext *self, const SysChar * progname);
const SysChar * cst_context_get_progname(CstContext *self);

void cst_context_set_v_component(CstContext *self, CstComponent * v_component);
CstComponent * cst_context_get_v_component(CstContext *self);

void cst_context_set_v_module(CstContext *self, CstModule * v_module);
CstModule * cst_context_get_v_module(CstContext *self);

void cst_context_set_v_render(CstContext *self, CstRender * v_render);
CstRender * cst_context_get_v_render(CstContext *self);

SYS_END_DECLS

#endif
