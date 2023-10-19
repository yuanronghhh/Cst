#ifndef __CST_ROW_H__
#define __CST_ROW_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


struct _CstRow {
  SysSList parent;

  /* <private> */
  SysInt x;
  SysInt y;
};

CstRow *cst_row_new(void);
CstRow *cst_row_new_I(SysInt x, SysInt y);
CstRow *cst_row_add(CstRow *row, CstLayoutNode *lnode);

SYS_END_DECLS

#endif

