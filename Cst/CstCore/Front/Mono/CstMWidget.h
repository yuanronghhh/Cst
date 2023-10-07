#ifndef __CST_MLAYOUT_CONTEXT_H__
#define __CST_MLAYOUT_CONTEXT_H__

#include <CstCore/Front/Mono/CstMNode.h>

SYS_BEGIN_DECLS

#define _CST_TYPE_LAYOUT_CONTEXT (cst_widget_get_type())
#define _CST_LAYOUT_CONTEXT(o) ((CstLayoutContext* )o)

SysType cst_widget_get_type(void);
CstNode* _cst_widget_new(void);

SYS_END_DECLS

#endif

