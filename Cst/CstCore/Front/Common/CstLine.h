#ifndef __CST_LINE_H__
#define __CST_LINE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

typedef struct _CstLine CstLine;

CstLine *cst_line_new(void);
void cst_line_free(CstLine * self);
void cst_line_prepend(CstLine* self, CstNode* v_parent, CstNode* node);
void cst_line_set_start(CstLine* self, SysInt x, SysInt y);

SYS_END_DECLS

#endif

