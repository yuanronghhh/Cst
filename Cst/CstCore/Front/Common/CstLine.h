#ifndef __CST_LINE_H__
#define __CST_LINE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

typedef struct _CstLine CstLine;

CstLine* cst_line_new(SysInt x, SysInt y);
void cst_line_free(CstLine * self);
void cst_line_prepend_data(CstLine* self, CstNode* node);
SysBool cst_line_need_wrap (CstLine *self, SysInt append_width, SysInt width) ;
void cst_line_get_maxsize(CstLine* self, SysInt* max_w, SysInt* max_h);
void cst_line_set_size(CstLine* self, SysInt width, SysInt height);
void cst_line_get_size(CstLine* self, SysInt* width, SysInt* height);
const FRRect* cst_line_get_bound(CstLine* self);

SysSList* cst_lines_prepend(SysSList* lines, CstLine* line);

SYS_END_DECLS

#endif

