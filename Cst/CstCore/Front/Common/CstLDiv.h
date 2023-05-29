#ifndef __CST_LDIV_H__
#define __CST_LDIV_H__

#include <CstCore/Front/Common/CstWidget.h>

SYS_BEGIN_DECLS

#define  CST_TYPE_LDIV          _CST_TYPE_LDIV
#define  CST_LDIV_CLASS(cls)    _CST_LDIV_CLASS(cls)
#define  CST_LDIV_GET_CLASS(o)  _CST_LDIV_GET_CLASS(o)
#define  CST_LDIV(o)            _CST_LDIV(o)

SYS_API SysType cst_ldiv_get_type(void);
SYS_API CstNode* cst_ldiv_new(void);

SYS_END_DECLS

#endif

