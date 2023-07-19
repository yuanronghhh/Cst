#ifndef __CST_LINE_H__
#define __CST_LINE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

typedef struct _CstLine CstLine;

void cst_line_set_xy(CstLine* self, SysInt x, SysInt y);
SysBool cst_line_need_wrap(CstLine* self, SysInt append_width, SysInt max_width);
CstLine* cst_line_new(SysInt x, SysInt y);
void cst_line_free(CstLine * self);
void cst_line_clear(CstLine* self);
void cst_line_prepend_data_h(CstLine* self, CstNode* node);
SysSList* cst_line_layout_nodes(CstLine* line, SysSList* lines, const FRRect* pbound, SysBool parent_wrap);
SysList* cst_line_get_nodes(CstLine* self);
void cst_line_get_maxsize(CstLine* self, SysInt* max_w, SysInt* max_h);
void cst_line_set_size(CstLine* self, SysInt width, SysInt height);
void cst_line_get_size(CstLine* self, SysInt* width, SysInt* height);
const FRRect* cst_line_get_bound(CstLine* self);

SysSList* cst_lines_prepend(SysSList* lines, CstLine* line);

SYS_END_DECLS

#endif

