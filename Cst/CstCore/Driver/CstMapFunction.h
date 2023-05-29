#ifndef __CST_AWATCH_H__
#define __CST_AWATCH_H__

#include "Driver/CstCommon.h"

SYS_BEGIN_DECLS

#if 0
struct _CstMapCssValue {
  FRAWatch parent;

  SysChar *name;
  CST_TYPE_CSS_PROP type;
  CstCssCalcFunc func;
};

struct _CstMapAction {
  FRAWatch parent;

  SysChar *name;
  CstNode *node;
  FR_TYPE_ACTION type;
  FRActionFunc func;
};

struct _CstMapFunction {
  FRAWatch parent;

  SysChar *key_name;
  SysChar *func_name;
  SysFunc func;
};
#endif

SYS_END_DECLS

#endif
