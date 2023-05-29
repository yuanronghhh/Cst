#ifndef __CST_LBOX_H__
#define __CST_LBOX_H__

#include <CstCore/Front/Common/CstWidget.h>

SYS_BEGIN_DECLS

#define  CST_TYPE_LBOX          _CST_TYPE_LBOX
#define  CST_LBOX_CLASS(cls)    _CST_LBOX_CLASS(cls)
#define  CST_LBOX_GET_CLASS(o)  _CST_LBOX_GET_CLASS(o)
#define  CST_LBOX(o)            _CST_LBOX(o)


SysType cst_lbox_get_type(void);
CstNode* cst_lbox_new(void);

SYS_END_DECLS

#endif

