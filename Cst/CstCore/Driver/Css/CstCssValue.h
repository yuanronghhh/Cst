#ifndef __CST_CSS_VALUE_H__
#define __CST_CSS_VALUE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS


#define CST_TYPE_CSS_VALUE (cst_css_value_get_type())
#define CST_CSS_VALUE(o) ((CstCssValue* )sys_object_cast_check(o, CST_TYPE_CSS_VALUE))
#define CST_CSS_VALUE_CLASS(o) ((CstCssValueClass *)sys_class_cast_check(o, CST_TYPE_CSS_VALUE))
#define CST_CSS_VALUE_GET_CLASS(o) sys_instance_get_class(o, CstCssValueClass)

typedef union _CstCssValueV CstCssValueV;

union _CstCssValueV {
  SysBool v_bool;
  SysDouble v_double;
  SysInt v_int;
  SysChar* v_string;

  SysPointer v_pointer;
  FRSInt4 *v_m4;
  FRColor *v_color;
  CstCssClosure *v_closure;
};

struct _CstCssValue {
  SysObject parent;

  /* <private> */
  SysChar *name;
  /* CST_CSS_PROP_ENUM */
  SysInt css_type;
  /* CST_RENDER_STATE_ENUM */
  SysInt state_flag;
  CST_CSS_VALUE_ENUM d_type;
  CstCssValueV v;
};

struct _CstCssValueClass {
  SysObjectClass parent;

  void (*construct) (CstCssValue *self, SysChar *name, SysInt css_type, SysInt state_flag);
  SysInt (*parse) (JNode *jnode, CstCssValue *selfalue);
  void (*calc) (CstCssValue *self, CstRenderNode *render_node, CstLayout *layout);
  void (*set_value)(CstCssValue *self, CstRenderNode *render_node, CstLayout *layout);
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

CstCssClosure* cst_css_value_parse_calc(SysChar * s, CstCssCalcFunc func);
void cst_css_value_width_percent(CstRenderNode *render_node, SysPointer user_data);
void cst_css_value_height_percent(CstRenderNode *render_node, SysPointer user_data);

SYS_API SysType cst_css_value_node_lookup(const SysChar *name);
SYS_API const SysChar *cst_css_value_node_name(CstCssValue *self);
SYS_API SysInt cst_css_value_parse(JNode *jnode, CstCssValue *value);

SYS_API void cst_css_value_setup(void);
SYS_API void cst_css_value_teardown(void);
SYS_API CstCssValue *cst_css_value_dclone(CstCssValue *o);
SYS_API SysInt cst_css_value_get_state(CstCssValue *self);
SYS_API void cst_css_value_calc(CstCssValue *self, CstRenderNode *render_node, CstLayout *layout);
SYS_API void cst_css_value_set_value(CstCssValue *self, CstRenderNode *render_node, CstLayout *layout);

SYS_END_DECLS

#endif


