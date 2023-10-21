#ifndef __CST_ROW_H__
#define __CST_ROW_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


struct _CstRow {
  SysSList parent;

  /* <private> */
  FRRect bound;
};

CstRow *cst_row_new(void);
CstRow *cst_row_new_I(SysInt x, SysInt y);

const FRRect *cst_row_get_bound(CstRow* self);
void cst_row_add(CstRow* self, CstLayoutNode* lnode);
void cst_row_free(CstRow *self);

SYS_END_DECLS

#endif

