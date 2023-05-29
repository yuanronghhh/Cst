#ifndef __CST_CLGRID_H__
#define __CST_CLGRID_H__

#include <CstCore/Front/C/CstCWidget.h>

SYS_BEGIN_DECLS

struct _CstLGrid {
  CstWidget parent;
  CstLGridPrivate* priv;
};

struct _CstLGridClass {
  CstWidgetClass parent;
};

SYS_END_DECLS

#endif

