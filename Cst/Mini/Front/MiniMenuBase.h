#ifndef __MINI_MENUBASE_H__
#define __MINI_MENUBASE_H__

/***
 * MiniMenuBase auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define MINI_TYPE_MENUBASE (mini_menubase_get_type())
#define MINI_MENUBASE(o) ((MiniMenuBase* )o)
#define MINI_MENUBASE_CLASS(o) ((MiniMenuBaseClass *)o)
#define MINI_MENUBASE_GET_CLASS(o) sys_instance_get_class(o, MiniMenuBaseClass)

typedef struct _MiniMenuBase MiniMenuBase;
typedef struct _MiniMenuBaseClass MiniMenuBaseClass;
typedef struct _MiniMenuBasePrivate MiniMenuBasePrivate;

struct _MiniMenuBase {
  CstComponent parent;
  MiniMenuBasePrivate *priv;
};

struct _MiniMenuBaseClass {
  CstComponentClass parent;
};

SYS_API SysType mini_menubase_get_type(void);
SYS_API MiniMenuBase* mini_menubase_new(void);

SYS_END_DECLS

#endif
