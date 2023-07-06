#ifndef __CST_LINE_H__
#define __CST_LINE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

typedef struct _CstLine CstLine;

CstLine* cst_line_new(SysInt offset_w, SysInt offset_h);
void cst_line_free(CstLine * self);
void cst_line_prepend_data(CstLine* self, CstNode* node);
void cst_line_get_offsize(CstLine* self, SysInt* offset_w, SysInt* offset_h);
SysBool cst_line_need_wrap (CstLine *self, SysInt append_width, SysInt width) ;
void cst_line_get_maxsize(CstLine* self, SysInt* max_w, SysInt* max_h);
void cst_line_set_offsize(CstLine* self, SysInt offset_w, SysInt offset_h);

SysSList* cst_lines_prepend(SysSList* lines, CstLine* line);

SYS_END_DECLS

#endif

