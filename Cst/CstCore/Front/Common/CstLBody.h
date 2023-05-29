#ifndef __CST_LBODY_H__
#define __CST_LBODY_H__

#include <CstCore/Front/Common/CstWidget.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LBODY _CST_TYPE_LBODY
#define CST_LBODY_CLASS(cls) _CST_LBODY_CLASS(cls)
#define CST_LBODY_GET_CLASS(o) _CST_LBODY_GET_CLASS(o)
#define CST_LBODY(o) _CST_LBODY(o)

SysType cst_lbody_get_type(void);
CstNode* cst_lbody_new(void);

SYS_END_DECLS

#endif

