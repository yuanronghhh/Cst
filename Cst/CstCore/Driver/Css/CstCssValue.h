#ifndef __CST_CSS_VALUE_H__
#define __CST_CSS_VALUE_H__

#include <CstCore/Parser/CstParserCommon.h>

SYS_BEGIN_DECLS

void cst_css_value_setup(void);
void cst_css_value_teardown(void);

SYS_API CstCssValueNode *cst_css_value_node_lookup(const SysChar* name);
const SysChar * cst_css_value_node_name(CstCssValueNode * node);

void cst_css_value_set_bool(CstCssValue * value, SysBool v);
void cst_css_value_set_string(CstCssValue * value, const SysChar * v);
void cst_css_value_set_null(CstCssValue * value, SysPointer v);
void cst_css_value_set_pointer(CstCssValue * value, SysPointer v);
void cst_css_value_set_m4(CstCssValue * value, SysInt * m4);
void cst_css_value_set_int(CstCssValue * value, SysInt v);
void cst_css_value_set_double(CstCssValue * value, SysDouble v);
void cst_css_value_set_color(CstCssValue * value, FRColor * v);
void cst_css_value_set_closure(CstCssValue * value, CstCssClosure * v);
CstCssValue * cst_css_value_new(CstCssValueNode * node);
CST_CSS_GTYPE_ENUM cst_css_value_get_g_type(CstCssValue * value);
SysInt cst_css_value_parse(JNode * jnode, CstCssValue * value);

SYS_API void cst_css_value_node_add(const SysChar* name, CstCssValueNode *node);
CstCssValueNode * cst_css_value_get_node(CstCssValue * value);

SYS_API SysInt cst_css_value_layout(CstCssValue *value, FRContext *cr, CstNode *w);
SYS_API SysInt cst_css_value_paint(CstCssValue *value, FRContext *cr, CstNode *w);

SYS_API void cst_css_value_free(CstCssValue *value);
CstCssValue * cst_css_value_clone(CstCssValue * value);

void cst_css_value_width_percent(CstNode * v_parent, CstNode * v_node, FRContext * cr, SysPointer value);
void cst_css_value_height_percent(CstNode * v_parent, CstNode * v_node, FRContext * cr, SysPointer value);
CstCssClosure * cst_css_value_parse_calc(SysChar * s, CstCssCalcFunc func);

SYS_END_DECLS

#endif
