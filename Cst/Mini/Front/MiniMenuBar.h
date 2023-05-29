#ifndef __MINI_MENUBAR_H__
#define __MINI_MENUBAR_H__

/***
 * MiniMenuBar auto generated from ComponentCode.header
 */

#include <CstCore/CstCore.h>

SYS_BEGIN_DECLS

#define MINI_TYPE_MENUBAR (mini_menubar_get_type())
#define MINI_MENUBAR(o) ((MiniMenuBar* )o)
#define MINI_MENUBAR_CLASS(o) ((MiniMenuBarClass *)o)
#define MINI_MENUBAR_GET_CLASS(o) sys_instance_get_class(o, MiniMenuBarClass)

typedef struct _MiniMenuBar MiniMenuBar;
typedef struct _MiniMenuBarClass MiniMenuBarClass;
typedef struct _MiniMenuBarPrivate MiniMenuBarPrivate;

struct _MiniMenuBar {
  CstComponent parent;
  MiniMenuBarPrivate *priv;
};

struct _MiniMenuBarClass {
  CstComponentClass parent;
};

SYS_API SysType mini_menubar_get_type(void);
SYS_API MiniMenuBar* mini_menubar_new(void);

SYS_END_DECLS

#endif
