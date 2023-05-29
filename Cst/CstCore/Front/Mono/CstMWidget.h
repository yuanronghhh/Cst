#ifndef __CST_MWIDGET_H__
#define __CST_MWIDGET_H__

#include <CstCore/Front/Mono/CstMNode.h>

SYS_BEGIN_DECLS

#define _CST_TYPE_WIDGET (cst_widget_get_type())
#define _CST_WIDGET(o) ((CstWidget* )o)

SysType cst_widget_get_type(void);
CstNode* _cst_widget_new(void);

SYS_END_DECLS

#endif

