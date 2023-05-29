#ifndef __CST_CCOM_NODE_H__
#define __CST_CCOM_NODE_H__

#include <CstCore/Front/Common/CstComNode.h>
#include <CstCore/Front/C/CstCNode.h>

SYS_BEGIN_DECLS

struct _CstComNode {
  CstNode parent;
  CstComNodePrivate* priv;
};

struct _CstComNodeClass {
  CstNodeClass parent;
};


SYS_END_DECLS

#endif

