#ifndef __CST_CTEXT_H__
#define __CST_CTEXT_H__

#include <CstCore/Front/C/CstCWidget.h>

SYS_BEGIN_DECLS

struct _CstText {
  CstWidget parent;

  CstTextPrivate* priv;
};

struct _CstTextClass {
  CstWidgetClass parent;
};

SYS_END_DECLS

#endif

