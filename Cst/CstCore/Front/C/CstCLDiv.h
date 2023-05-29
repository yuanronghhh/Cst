#ifndef __CST_CLDIV_H__
#define __CST_CLDIV_H__

#include <CstCore/Front/C/CstCWidget.h>

SYS_BEGIN_DECLS

struct _CstLDiv {
  CstWidget parent;
  CstLDivPrivate* priv;
};

struct _CstLDivClass {
  CstWidgetClass parent;
};

SYS_END_DECLS

#endif

