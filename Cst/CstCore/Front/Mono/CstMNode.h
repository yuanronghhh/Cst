#ifndef __CST_MNODE_H__
#define __CST_MNODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define _CST_TYPE_NODE (cst_node_get_type())
#define _CST_NODE(o) ((CstNode* )o)

CstNode* _cst_node_new(void);

SYS_END_DECLS

#endif

