#ifndef __CST_LFLEX_H__
#define __CST_LFLEX_H__

#include <CstCore/Front/Common/CstWidget.h>

SYS_BEGIN_DECLS

#define  CST_TYPE_LFLEX          _CST_TYPE_LFLEX
#define  CST_LFLEX_CLASS(cls)    _CST_LFLEX_CLASS(cls)
#define  CST_LFLEX_GET_CLASS(o)  _CST_LFLEX_GET_CLASS(o)
#define  CST_LFLEX(o)            _CST_LFLEX(o)

SYS_API SysType cst_lflex_get_type(void);
SYS_API CstNode* cst_lflex_new(void);

SYS_END_DECLS

#endif

