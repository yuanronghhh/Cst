#ifndef __CST_WIDGET_H__
#define __CST_WIDGET_H__

#include <CstCore/Front/Common/CstNode.h>

SYS_BEGIN_DECLS

#define CST_TYPE_WIDGET _CST_TYPE_WIDGET
#define CST_WIDGET(o) _CST_WIDGET(o)
#define CST_WIDGET_CLASS(o) _CST_WIDGET_CLASS(o)
#define CST_WIDGET_GET_CLASS(o) _CST_WIDGET_GET_CLASS(o)

SYS_API CstNode* cst_widget_new(void);
SYS_API SysType cst_widget_get_type(void);


SYS_END_DECLS

#endif

