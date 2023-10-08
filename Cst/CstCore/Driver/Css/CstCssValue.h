#ifndef __CST_CSS_VALUE_H__
#define __CST_CSS_VALUE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_VALUE (cst_css_value_get_type())
#define CST_CSS_VALUE(o) ((CstCssValue* )sys_object_cast_check(o, CST_TYPE_CSS_VALUE))
#define CST_CSS_VALUE_CLASS(o) ((CstCssValueClass *)sys_class_cast_check(o, CST_TYPE_CSS_VALUE))
#define CST_CSS_VALUE_GET_CLASS(o) sys_instance_get_class(o, CstCssValueClass)


struct _CstCssValueClass {
  SysObjectClass parent;

  void (*construct) (CstCssValue *self, SysInt state_flag);
  void (*set_value) (CstCssValue* self, CstRenderNode* render_node, CstLayout* layout);
  CstCssValue* (*parse) (CstCssParser *parser);
  CstCssValue *(*dclone)(CstCssValue *o);
};


SYS_API SysType cst_css_value_get_type(void);
SYS_API CstCssValue *cst_css_value_new(void);

void cst_css_value_set_bool(CstCssValue* value, SysBool v);
void cst_css_value_set_string(CstCssValue* value, const SysChar* v);
void cst_css_value_set_null(CstCssValue* value, SysPointer v);
void cst_css_value_set_pointer(CstCssValue* value, SysPointer v);
void cst_css_value_set_m4(CstCssValue* value, FRSInt4* m4);
void cst_css_value_set_int(CstCssValue* value, SysInt v);
void cst_css_value_set_double(CstCssValue* value, SysDouble v);
void cst_css_value_set_color(CstCssValue* value, FRColor* v);
void cst_css_value_set_closure(CstCssValue* value, CstCssClosure* v);
SYS_API CstCssValue *cst_css_value_dclone(CstCssValue *o);
SYS_API void cst_css_value_set_value(CstCssValue* self, CstRenderNode* render_node, CstLayout* layout);

SYS_API CstCssClosure* cst_css_value_parse_calc(SysChar* s, CstCssCalcFunc func);
SYS_API void cst_css_value_width_percent(CstRenderNode* render_node, SysPointer user_data);
SYS_API void cst_css_value_height_percent(CstRenderNode* render_node, SysPointer user_data);

SYS_END_DECLS

#endif


