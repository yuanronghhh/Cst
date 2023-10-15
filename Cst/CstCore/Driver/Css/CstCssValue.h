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
  CstCssValue* (*parse) (CstCssParser *parser);
  CstCssValue *(*dclone)(CstCssValue *o);
};


SysType cst_css_value_get_type(void);
CstCssValue *cst_css_value_new(void);

CstCssValue *cst_css_value_dclone(CstCssValue *o);

CstCssClosure* cst_css_value_parse_percent(SysChar* s, CstCssFunc func);

void cst_css_value_set_v_int(CstCssValue *self, SysInt v_int);
SysInt cst_css_value_get_v_int(CstCssValue *self);

void cst_css_value_set_v_bool(CstCssValue *self, SysBool v_bool);
SysBool cst_css_value_get_v_bool(CstCssValue *self);

void cst_css_value_set_v_string(CstCssValue *self, const SysChar* v_string);
const SysChar* cst_css_value_get_v_string(CstCssValue *self);

void cst_css_value_set_v_null(CstCssValue *self);

void cst_css_value_set_v_pointer(CstCssValue *self, SysPointer v_pointer);
SysPointer cst_css_value_get_v_pointer(CstCssValue *self);

void cst_css_value_set_v_m4(CstCssValue *self, FRSInt4* v_m4);
FRSInt4* cst_css_value_get_v_m4(CstCssValue *self);

void cst_css_value_set_v_double(CstCssValue *self, SysDouble v_double);
SysDouble cst_css_value_get_v_double(CstCssValue *self);

void cst_css_value_set_v_color(CstCssValue *self, FRColor* v_color);
FRColor* cst_css_value_get_v_color(CstCssValue *self);

void cst_css_value_set_v_closure(CstCssValue *self, CstCssClosure* v_closure);
CstCssClosure* cst_css_value_get_v_closure(CstCssValue *self);

SysBool cst_css_value_is_d_type(CstCssValue * self, SysInt tp);

SYS_END_DECLS

#endif


