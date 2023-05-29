#ifndef __CST_CLFLEX_H__
#define __CST_CLFLEX_H__

#include <CstCore/Front/C/CstCWidget.h>
#include <CstCore/Front/Common/CstLFlex.h>

SYS_BEGIN_DECLS

#define _CST_TYPE_LFLEX (cst_lflex_get_type())
#define _CST_LFLEX_CLASS(cls) ((CstLFlexClass *)cls)
#define _CST_LFLEX_GET_CLASS(o) sys_instance_get_class(o, CstLFlexClass)
#define _CST_LFLEX(o) ((CstLFlex* )o)

struct _CstLFlex {
  CstWidget parent;
  CstLFlexPrivate* priv;
};

struct _CstLFlexClass {
  CstWidgetClass parent;
};

SYS_END_DECLS

#endif

