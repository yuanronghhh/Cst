#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Driver/CstLayout.h>

union _CstCssValueV {
  SysBool v_bool;
  SysDouble v_double;
  SysInt v_int;
  SysChar* v_string;

  SysPointer v_pointer;
  FRSInt4* v_m4;
  FRColor* v_color;
  CstCssClosure* v_closure;
};

struct _CstCssValue {
  SysObject parent;

  /* <private> */
  SysUInt d_type;
  CstCssValueV v;
};

SYS_DEFINE_TYPE(CstCssValue, cst_css_value, SYS_TYPE_OBJECT);

CstCssValue *cst_css_value_dclone(CstCssValue *self) {
  CstCssValueClass *cls = CST_CSS_VALUE_GET_CLASS(self);

  sys_return_val_if_fail(cls->dclone != NULL, NULL);
  return cls->dclone(self);
}

void cst_css_value_set_value(CstCssValue *self, CstRenderNode *render_node, CstLayout *layout) {
  CstCssValueClass *cls = CST_CSS_VALUE_GET_CLASS(self);

  sys_return_if_fail(cls->set_value != NULL);
  cls->set_value(self, render_node, layout);
}

void cst_css_value_set_bool(CstCssValue* value, SysBool v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_BOOL;
  value->v.v_bool = v;
}

void cst_css_value_set_string(CstCssValue* value, const SysChar*  v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_STRING;
  value->v.v_string = sys_strdup(v);
}

void cst_css_value_set_null(CstCssValue* value, SysPointer v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_NULL;
  value->v.v_pointer = NULL;
}

void cst_css_value_set_pointer(CstCssValue* value, SysPointer v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_POINTER;
  value->v.v_pointer = v;
}

void cst_css_value_set_m4(CstCssValue* value, FRSInt4* m4) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_M4;
  value->v.v_m4 = m4;
}

void cst_css_value_set_int(CstCssValue* value, SysInt v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_INT;
  value->v.v_int = v;
}

void cst_css_value_set_double(CstCssValue* value, SysDouble v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_DOUBLE;
  value->v.v_double = v;
}

void cst_css_value_set_color(CstCssValue* value, FRColor* v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_COLOR;
  value->v.v_color = v;
}

void cst_css_value_set_closure(CstCssValue* value, CstCssClosure* v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_CLOSURE;
  value->v.v_closure = v;
}

CstCssClosure* cst_css_value_parse_calc(SysChar *s, CstCssCalcFunc func) {
  sys_return_val_if_fail(s != NULL, NULL);

  SysSize slen = sys_strlen(s, 5);
  SysInt64 r = 0;

  if (*(s + (slen - 1)) == '%') {
    *(s + (slen - 1)) = '\0';

     r = sys_str_to_int64(s);

  } else {

    return NULL;
  }

  return cst_css_closure_new_I(INT_TO_POINTER(r), func, NULL);
}

void cst_css_value_width_percent(CstRenderNode *render_node, SysPointer user_data) {
  sys_return_if_fail(render_node != NULL);

  const FRRect *bound;
  SysInt pwidth;
  FRSInt4 m4;
  SysInt64 d;
  CstRenderNode *pnode;

  d = POINTER_TO_INT(user_data);
  pnode =  cst_render_node_get_parent(render_node);
  bound = cst_render_node_get_bound(pnode);

  cst_render_node_get_mbp(render_node, &m4);
  pwidth = bound->width - m4.m1 - m4.m3;

  cst_render_node_set_width(render_node, (SysInt)(pwidth * d * 0.01));
}

void cst_css_value_height_percent(CstRenderNode *render_node, SysPointer user_data) {
  sys_return_if_fail(render_node != NULL);

  const FRRect *bound;
  SysInt pheight;
  FRSInt4 m4;
  SysInt64 d;

  CstRenderNode *pnode;

  d = POINTER_TO_INT(user_data);
  pnode = cst_render_node_get_parent(render_node);
  bound = cst_render_node_get_bound(pnode);

  cst_render_node_get_mbp(render_node, &m4);
  pheight = bound->height - m4.m0 - m4.m2;

  cst_render_node_set_height(render_node, (SysInt)(pheight * d * 0.01));
}

/* object api */
static void cst_css_value_dispose(SysObject* o) {
  CstCssValue *self = CST_CSS_VALUE(o);

  SYS_OBJECT_CLASS(cst_css_value_parent_class)->dispose(o);
}

CstCssValue *cst_css_value_new(void) {
  return sys_object_new(CST_TYPE_CSS_VALUE, NULL);
}

static void cst_css_value_class_init(CstCssValueClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_value_dispose;
}

static void cst_css_value_init(CstCssValue *self) {
}
