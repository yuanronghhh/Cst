#ifndef __CST_LAYOUT_H__
#define __CST_LAYOUT_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

typedef struct _CstLayout CstLayout;
typedef struct _CstLayoutContext CstLayoutContext;

void cst_layout_set_xy(CstLayout* self, SysInt x, SysInt y);
SysBool cst_layout_need_wrap(CstLayout* self, SysInt append_width, SysInt max_width);
CstLayout* cst_layout_new(SysInt x, SysInt y);
CstLayoutContext* cst_layout_context_new(void);
void cst_layout_free(CstLayout * self);
void cst_layout_clear(CstLayout* self);
void cst_layout_layout_update(CstLayout* self);
void cst_layout_prepend_item(CstLayout* self, CstNode* node);
void cst_layout_layout_nodes(CstLayout* layout, const FRRect* pbound, SysBool parent_wrap);
SysList* cst_layout_get_nodes(CstLayout* self);
void cst_layout_set_size(CstLayout* self, SysInt width, SysInt height);
void cst_layout_get_size(CstLayout* self, SysInt* width, SysInt* height);
const FRRect* cst_layout_get_bound(CstLayout* self);
SysSList* cst_layouts_prepend(SysSList* layouts, CstLayout* layout);

SYS_END_DECLS

#endif

