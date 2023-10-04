#ifndef __CST_PAINT_CONTEXT__
#define __CST_PAINT_CONTEXT__

#include <CstCore/Driver/CstCommon.h>


SYS_BEGIN_DECLS

#define CST_TYPE_PAINT_CONTEXT (cst_paint_context_get_type())
#define CST_PAINT_CONTEXT(o) ((CstPaintContext* )sys_object_cast_check(o, CST_TYPE_PAINT_CONTEXT))
#define CST_PAINT_CONTEXT_CLASS(o) ((CstPaintContextClass *)sys_class_cast_check(o, CST_TYPE_PAINT_CONTEXT))
#define CST_PAINT_CONTEXT_GET_CLASS(o) sys_instance_get_class(o, CstPaintContextClass)

struct _CstPaintContext {
  SysObject parent;

  /* < private > */
  SysBool need_repaint;
};

struct _CstPaintContextClass {
  SysObjectClass parent;

  void (*paint_self) (CstPaintContext *ctx, CstPaintNode *item, CstLayout *layout);
  void (*paint_children) (CstPaintContext *ctx, CstPaintNode *item, CstLayout *layout);
};

SysType cst_paint_context_get_type(void);
CstPaintContext *cst_paint_context_new(void);
CstPaintContext *cst_paint_context_new_I(void);

void cst_paint_context_paint_self (CstPaintContext *self, CstPaintNode *item, CstLayout *layout);
void cst_paint_context_paint_children (CstPaintContext *self, CstPaintNode *item, CstLayout* layout);
SysBool cst_paint_context_need_paint(CstPaintContext* self);

SYS_END_DECLS

#endif

