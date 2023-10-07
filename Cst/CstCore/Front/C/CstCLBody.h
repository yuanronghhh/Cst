#ifndef __CST_CLBODY_H__
#define __CST_CLBODY_H__

#include <CstCore/Front/C/CstCNode.h>
#include <CstCore/Front/Common/CstLBody.h>

SYS_BEGIN_DECLS

struct _CstLBody {
  CstNode parent;
};

struct _CstLBodyClass {
  CstNodeClass parent;
};

SYS_END_DECLS

#endif

