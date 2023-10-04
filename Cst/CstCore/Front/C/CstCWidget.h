#ifndef __CST_CWIDGET_H__
#define __CST_CWIDGET_H__

#include <CstCore/Front/Common/CstWidget.h>
#include <CstCore/Front/C/CstCNode.h>

SYS_BEGIN_DECLS

struct _CstWidget {
  CstNode parent;

  /* <private> */
  CstNode *component_node;
};

struct _CstWidgetClass {
  CstNodeClass parent;
};

SYS_END_DECLS

#endif

