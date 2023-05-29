#ifndef __CST_BOX_H__
#define __CST_BOX_H__

#include <CstCore/Front/CstWidget.h>

SYS_BEGIN_DECLS

#define CST_BOX(o) ((CstBox* )o)
#define CST_TYPE_BOX (cst_box_get_type())

typedef struct _CstBox CstBox;
typedef struct _CstBoxClass CstBoxClass;
typedef struct _CstBoxPrivate CstBoxPrivate;

struct _CstBox {
  CstWidget parent;
  CstBoxPrivate* priv;
};

struct _CstBoxClass {
  CstWidgetClass parent;
};

SYS_API SysType cst_box_get_type(void);

SYS_END_DECLS

#endif

