#ifndef __CST_LGRID_H__
#define __CST_LGRID_H__

#include <CstCore/Front/Common/CstWidget.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LGRID _CST_TYPE_LGRID
#define CST_LGRID_CLASS(cls) _CST_LGRID_CLASS(cls)
#define CST_LGRID_GET_CLASS(o) _CST_LGRID_GET_CLASS(o)
#define CST_LGRID(o) _CST_LGRID(o)

CstNode* cst_lgrid_new(void);
SysType cst_lgrid_get_type(void);

SYS_END_DECLS

#endif

