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
  CST_CSS_VALUE_ENUM d_type;
  CstCssValueV v;
};

SYS_DEFINE_TYPE(CstCssValue, cst_css_value, SYS_TYPE_OBJECT);

CstCssValue *cst_css_value_dclone(CstCssValue *self) {
  CstCssValueClass *cls = CST_CSS_VALUE_GET_CLASS(self);

  sys_return_val_if_fail(cls->dclone != NULL, NULL);
  return cls->dclone(self);
}

SysBool cst_css_value_get_v_bool(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, false);

  return self->v.v_bool;
}

void cst_css_value_set_v_bool(CstCssValue* self, SysBool v) {
  sys_return_if_fail(self != NULL);

  self->d_type = CST_CSS_VALUE_BOOL;
  self->v.v_bool = v;
}

void cst_css_value_set_v_string(CstCssValue *self, const SysChar* v_string) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v_string != NULL);

  self->d_type = CST_CSS_VALUE_STRING;
  self->v.v_string = sys_strdup(v_string);
}

const SysChar* cst_css_value_get_v_string(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v.v_string;
}

void cst_css_value_set_v_null(CstCssValue *self) {
  sys_return_if_fail(self != NULL);

  self->d_type = CST_CSS_VALUE_NULL;
  self->v.v_pointer = NULL;
}

void cst_css_value_set_v_pointer(CstCssValue *self, SysPointer v_pointer) {
  sys_return_if_fail(self != NULL);

  self->d_type = CST_CSS_VALUE_POINTER;
  self->v.v_pointer = v_pointer;
}

SysPointer cst_css_value_get_v_pointer(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v.v_pointer;
}

void cst_css_value_set_v_m4(CstCssValue* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  self->d_type = CST_CSS_VALUE_M4;
  self->v.v_m4 = m4;
}

FRSInt4* cst_css_value_get_v_m4(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v.v_m4;
}

void cst_css_value_set_v_double(CstCssValue* self, SysDouble v) {
  sys_return_if_fail(self != NULL);

  self->d_type = CST_CSS_VALUE_DOUBLE;
  self->v.v_double = v;
}

SysDouble cst_css_value_get_v_double(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->v.v_double;
}

void cst_css_value_set_v_color(CstCssValue* self, FRColor* v) {
  sys_return_if_fail(self != NULL);

  self->d_type = CST_CSS_VALUE_COLOR;
  self->v.v_color = v;
}

FRColor* cst_css_value_get_v_color(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v.v_color;
}

void cst_css_value_set_v_closure(CstCssValue* self, CstCssClosure* v) {
  sys_return_if_fail(self != NULL);
  sys_return_if_fail(v != NULL);

  self->d_type = CST_CSS_VALUE_CLOSURE;
  self->v.v_closure = v;
}

SysBool cst_css_value_is_d_type(CstCssValue* self, SysInt tp) {
  return self->d_type == tp;
}

CstCssClosure* cst_css_value_get_v_closure(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->v.v_closure;
}

void cst_css_value_set_v_int(CstCssValue *self, SysInt v_int) {
  sys_return_if_fail(self != NULL);

  self->d_type = CST_CSS_VALUE_INT;
  self->v.v_int = v_int;
}

SysInt cst_css_value_get_v_int(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->v.v_int;
}

CstCssClosure* cst_css_value_parse_percent(SysChar *s, CstCssFunc func) {
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

/* object api */
static void cst_css_value_dispose(SysObject* o) {

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
