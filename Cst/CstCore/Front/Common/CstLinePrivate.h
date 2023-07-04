#ifndef __CST_LINE_PRIVATE_H__
#define __CST_LINE_PRIVATE_H__

#include <CstCore/Front/Common/CstLine.h>

SYS_BEGIN_DECLS


struct _CstLine {
  SysList *nodes;

  SysInt x;
  SysInt y;
  SysInt offset_w;
  SysInt offset_h;
};

SYS_END_DECLS

#endif

