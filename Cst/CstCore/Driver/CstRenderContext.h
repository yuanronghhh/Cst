#ifndef __CST_RENDER_CONTEXT__
#define __CST_RENDER_CONTEXT__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_LAYOUT_CONTEXT (cst_render_context_get_type())
#define CST_RENDER_CONTEXT(o) ((CstRenderContext* )sys_object_cast_check(o, CST_TYPE_LAYOUT_CONTEXT))
#define CST_RENDER_CONTEXT_CLASS(o) ((CstRenderContextClass *)sys_class_cast_check(o, CST_TYPE_LAYOUT_CONTEXT))
#define CST_RENDER_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstRenderContextClass)

struct _CstRenderContext {
  SysObject parent;

  /* < private > */
  SysBool need_relayout;
  SysBool need_repaint;
  SysBool is_visible;
  SysBool wrap;
  SysInt16 line_space;

  SysInt position;
  SysInt child_count;
  FRSInt4 mbp;

  SysInt prefer_height;
  SysInt prefer_width;

  // self constraint
  CstCssClosure  *width_calc;
  CstCssClosure  *height_calc;
};

struct _CstRenderContextClass {
  SysObjectClass parent;

  void (*layout_self) (CstRenderContext* self, CstRenderNode* rnode, CstLayout* layout);
  void (*layout_children) (CstRenderContext *self, CstRenderNode *rnode, CstLayout *layout);
};

SysType cst_render_context_get_type(void);
CstRenderContext *cst_render_context_new(void);
CstRenderContext *cst_render_context_new_I(void);

void cst_render_context_layout_self(CstRenderContext* self, CstRenderNode *rnode, CstLayout* layout);
void cst_render_context_layout_children(CstRenderContext* self, CstRenderNode *rnode, CstLayout* layout);
CstRenderContext* cst_render_context_dclone(CstRenderContext *o);

void cst_render_context_set_mbp(CstRenderContext* self, FRSInt4* m4);
const FRSInt4* cst_render_context_get_mbp(CstRenderContext* self);

void cst_render_context_set_width_closure(CstRenderContext* self, CstCssClosure* c);
void cst_render_context_set_height_closure(CstRenderContext* self, CstCssClosure* c);

void cst_render_context_set_prefer_size(CstRenderContext* self, SysInt width, SysInt height);
void cst_render_context_get_prefer_size(CstRenderContext* self, SysInt* width, SysInt* height);

SysBool cst_render_context_need_layout(CstRenderContext* self);
void cst_render_context_set_layout(CstRenderContext *self, SysBool bvalue);

void cst_render_context_calc_width(CstRenderContext * self, CstLayout * layout, CstRenderNode * rnode);
void cst_render_context_calc_height(CstRenderContext * self, CstLayout * layout, CstRenderNode * rnode);
void cst_render_context_calc_size(CstRenderContext * self, CstLayout * layout, CstRenderNode * rnode);

void cst_render_context_constraint_height(CstRenderContext *self, CstRenderContext *pctx, SysInt *height);
void cst_render_context_constraint_width(CstRenderContext *self, CstRenderContext *pctx, SysInt *width);

/* paint */
SysBool cst_render_context_need_paint(CstRenderContext *self);
SysBool cst_render_context_is_visible(CstRenderContext* self);
void cst_render_context_set_paint(CstRenderContext *self, SysBool bvalue);

void cst_render_context_setup(void);
void cst_render_context_teardown(void);

void cst_render_context_set_position(CstRenderContext *self, SysInt position);
SysInt cst_render_context_get_position(CstRenderContext *self);

void cst_render_context_set_wrap(CstRenderContext *self, SysBool wrap);
SysBool cst_render_context_get_wrap(CstRenderContext *self);

SYS_END_DECLS

#endif
