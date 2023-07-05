#ifndef __CST_LINE_H__
#define __CST_LINE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

typedef struct _CstLine CstLine;

CstLine* cst_line_new(SysInt offset_w, SysInt offset_h);
void cst_line_free(CstLine * self);
void cst_line_prepend_data(CstLine* self, CstNode* v_parent, CstNode* node);
void cst_line_set_start(CstLine* self, SysInt x, SysInt y);
void cst_line_get_offsize(CstLine* self, SysInt *width, SysInt *height) ;
SysBool cst_line_need_wrap (CstLine *self, SysInt append_width, SysInt width) ;

SYS_END_DECLS

#endif

