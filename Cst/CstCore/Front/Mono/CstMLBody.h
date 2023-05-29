#ifndef __CST_MLBODY_H__
#define __CST_MLBODY_H__

#include <CstCore/Front/Mono/CstMWidget.h>

SYS_BEGIN_DECLS

#define _CST_TYPE_LBODY (cst_lbody_get_type())
#define _CST_LBODY_CLASS(cls) ((CstLBodyClass *)cls)
#define _CST_LBODY(o) ((CstLBody* )o)

SysType cst_lbody_get_type(void);
CstNode* _cst_lbody_new(CstManager *manager, CstModule *module);

SYS_END_DECLS

#endif

