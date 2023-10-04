#ifndef __CST_LAYOUT_CONTEXT__
#define __CST_LAYOUT_CONTEXT__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT_CONTEXT (cst_layout_context_get_type())
#define CST_LAYOUT_CONTEXT(o) ((CstLayoutContext* )sys_object_cast_check(o, CST_TYPE_LAYOUT_CONTEXT))
#define CST_LAYOUT_CONTEXT_CLASS(o) ((CstLayoutContextClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT_CONTEXT))
#define CST_LAYOUT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstLayoutContextClass)

struct _CstLayoutContext {
  SysObject parent;

  /* < private > */
  CstLayoutContext* pctx;
  SysBool need_relayout;
  SysBool need_repaint;
  SysBool is_visible;
  SysBool wrap;
  SysInt16 line_space;

  SysInt child_count;
  FRSInt4 mbp;

  SysInt prefer_height;
  SysInt prefer_width;

  // self constraint
  CstCssClosure  *width_calc;
  CstCssClosure  *height_calc;

  // constraint for child, may be NULL.
  CstCssClosure  *child_width_calc;
  CstCssClosure  *child_height_calc;
};

struct _CstLayoutContextClass {
  SysObjectClass parent;

  void (*layout_self) (CstLayoutContext *self, CstLayoutContext* pself, CstLayoutNode *layout_node, CstLayout *layout);
  void (*layout_children) (CstLayoutContext *self, CstLayoutContext *pself, CstLayoutNode *layout_node, CstLayout *layout);
};

SysType cst_layout_context_get_type(void);
CstLayoutContext *cst_layout_context_new(void);
CstLayoutContext *cst_layout_context_new_I(void);

void cst_layout_context_layout_self(CstLayoutContext* self, CstLayoutContext* pself, CstLayoutNode* layout_node, CstLayout* layout);
void cst_layout_context_layout_children(CstLayoutContext* self, CstLayoutContext* pself, CstLayoutNode* layout_node, CstLayout* layout);

void cst_layout_context_set_mbp(CstLayoutContext* self, FRSInt4* m4);
const FRSInt4* cst_layout_context_get_mbp(CstLayoutContext* self);

void cst_layout_context_set_prefer_size(CstLayoutContext* self, SysInt width, SysInt height);
void cst_layout_context_get_prefer_size(CstLayoutContext* self, SysInt* width, SysInt* height);

SysBool cst_layout_context_need_layout(CstLayoutContext* self);
void cst_layout_context_calc_size(CstLayoutContext* self, CstLayoutNode* layout_node);
SysBool cst_layout_context_can_wrap(CstLayoutContext* self);


SYS_END_DECLS

#endif

