#ifndef __CST_FLEX_LINE__
#define __CST_FLEX_LINE__

#include <CstCore/Driver/CstFlexItem.h>

SYS_BEGIN_DECLS

struct _CstFlexLine {
  /* <private> */
  SysHArray items;
  SysInt cross_size;
};

CstFlexLine *cst_flex_line_new(void);
void cst_flex_line_free(CstFlexLine *line);
void cst_flex_item_add(CstFlexLine* self, CstFlexItem* item);
CstFlexLine* cst_flex_line_dclone(const CstFlexLine* line);

SYS_END_DECLS

#endif

