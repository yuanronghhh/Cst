#ifndef __CST_LAYOUT__
#define __CST_LAYOUT__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT (cst_layout_get_type())
#define CST_LAYOUT(o) ((CstLayout* )sys_object_cast_check(o, CST_TYPE_LAYOUT))
#define CST_LAYOUT_CLASS(o) ((CstLayoutClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT))
#define CST_LAYOUT_GET_CLASS(o) sys_instance_get_class(o, CstLayoutClass)

struct _CstLayout {
  SysObject parent;

  /* < private > */
  CST_RENDER_STATE_ENUM state;
};

struct _CstLayoutClass {
  SysObjectClass parent;

  void (*construct) (CstLayout* o);
};

SysType cst_layout_get_type(void);
CstLayout *cst_layout_new(void);
CstLayout *cst_layout_new_I(void);
CST_RENDER_STATE_ENUM cst_layout_get_state(CstLayout *self);
void cst_layout_set_flag(CstLayout *self, CST_RENDER_STATE_ENUM state);
SysBool cst_layout_state_layout(CstLayout *self);
void cst_layout_layout_children(CstLayout *self, CstLayoutNode *lnode, FRDraw *draw);

SYS_END_DECLS

#endif

