#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/CstFrontCore.h>


struct _CstCssValueClass {
  CstCssValueNode *node;
  SysInt          (*parse) (JNode *jnode, CstCssValue *value);
  SysInt          (*layout) (CstNode *node, FRContext *cr, CstCssValue *v);
  SysInt          (*paint) (CstNode *node, FRContext *cr, CstCssValue *v);
};

struct _CstCssValue {
  CstCssValueClass *cls;

  CST_CSS_VALUE_ENUM d_type;

  union CstCssValueV {
    SysBool v_bool;
    SysDouble v_double;
    SysInt v_int;
    SysChar* v_string;

    SysPointer v_pointer;
    FRSInt4 *v_m4;
    FRColor *v_color;
    CstCssClosure *v_closure;
  } v;

  SysRef ref_count;
};

struct _CstCssValueNode {
  SysChar *name;
  
  CST_RENDER_STATE_ENUM g_type;
  CST_CSS_PROP_ENUM p_type;

  CstCssValueClass *cls_ptr;
};

static SysHashTable *gcss_node_ht = NULL;

static CstCssValueNode x_node;
static CstCssValueNode y_node;
static CstCssValueNode padding_node;
static CstCssValueNode margin_node;
static CstCssValueNode width_node;
static CstCssValueNode height_node;
static CstCssValueNode color_node;
static CstCssValueNode font_family_node;
static CstCssValueNode font_size_node;
static CstCssValueNode wrap_node;

void cst_css_value_set_bool(CstCssValue *value, SysBool v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_BOOL;
  value->v.v_bool = v;
}

void cst_css_value_set_string(CstCssValue *value, const SysChar * v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_STRING;
  value->v.v_string = sys_strdup(v);
}

void cst_css_value_set_null(CstCssValue *value, SysPointer v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_NULL;
  value->v.v_pointer = NULL;
}

void cst_css_value_set_pointer(CstCssValue *value, SysPointer v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_POINTER;
  value->v.v_pointer = v;
}

void cst_css_value_set_m4(CstCssValue *value, FRSInt4 *m4) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_M4;
  value->v.v_m4 = m4;
}

void cst_css_value_set_int(CstCssValue *value, SysInt v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_INT;
  value->v.v_int = v;
}

void cst_css_value_set_double(CstCssValue *value, SysDouble v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_DOUBLE;
  value->v.v_double = v;
}

void cst_css_value_set_color(CstCssValue *value, FRColor *v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_COLOR;
  value->v.v_color = v;
}

void cst_css_value_set_closure(CstCssValue *value, CstCssClosure *v) {
  sys_return_if_fail(value != NULL);

  value->d_type = CST_CSS_VALUE_CLOSURE;
  value->v.v_closure = v;
}

CstCssValueNode * cst_css_value_get_node(CstCssValue *value) {
  sys_return_val_if_fail(value != NULL, NULL);

  return value->cls->node;
}

CstCssValue* cst_css_value_new(CstCssValueNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);
  sys_return_val_if_fail(node->cls_ptr != NULL, NULL);

  CstCssValue *v = sys_new0_N(CstCssValue, 1);
  v->cls = node->cls_ptr;

  sys_ref_count_init(v);

  return v;
}

CST_RENDER_STATE_ENUM cst_css_value_get_g_type(CstCssValue *value) {
  return value->cls->node->g_type;
}

SysInt cst_css_value_parse(JNode *jnode, CstCssValue *value) {
  return value->cls->parse(jnode, value);
}

SysInt cst_css_value_layout(CstCssValue *value, FRContext *cr, CstNode *node) {
  return value->cls->layout(node, cr, value);
}

SysInt cst_css_value_paint(CstCssValue *value, FRContext *cr, CstNode *w) {
  return value->cls->paint(w, cr, value);
}

void cst_css_value_free(CstCssValue *value) {
  switch (value->d_type) {
  case CST_CSS_VALUE_BOOL:
  case CST_CSS_VALUE_NULL:
  case CST_CSS_VALUE_DOUBLE:
  case CST_CSS_VALUE_INT:
    break;
  case CST_CSS_VALUE_STRING:
    sys_free_N(value->v.v_string);
    break;
  case CST_CSS_VALUE_COLOR:
    sys_free_N(value->v.v_color);
    break;
  case CST_CSS_VALUE_M4:
    sys_free_N(value->v.v_m4);
    break;
  case CST_CSS_VALUE_POINTER:
    break;
  case CST_CSS_VALUE_CLOSURE:
    sys_clear_pointer(&value->v.v_closure, cst_css_closure_unref);
    break;
  }

  sys_free_N(value);
}

CstCssValue *cst_css_value_clone(CstCssValue *value) {
  sys_return_val_if_fail(value != NULL, NULL);

  CstCssValueNode *node = cst_css_value_get_node(value);
  CstCssValue *nvalue = cst_css_value_new(node);

  nvalue->d_type = value->d_type;

  switch (value->d_type) {
    case CST_CSS_VALUE_BOOL:
    case CST_CSS_VALUE_NULL:
    case CST_CSS_VALUE_DOUBLE:
    case CST_CSS_VALUE_INT:
      nvalue->v.v_int = value->v.v_int;
      break;
    case CST_CSS_VALUE_STRING:
      nvalue->v.v_string = sys_strdup(value->v.v_string);
      break;
    case CST_CSS_VALUE_COLOR:
      nvalue->v.v_color = sys_memdup(value->v.v_color, sizeof(FRColor));
      break;
    case CST_CSS_VALUE_M4:
      nvalue->v.v_m4 = sys_memdup(value->v.v_m4, sizeof(SysInt) * 4);
      break;
    case CST_CSS_VALUE_POINTER:
      nvalue->v.v_pointer = value->v.v_pointer;
      break;
    case CST_CSS_VALUE_CLOSURE:
      nvalue->v.v_closure = cst_css_closure_clone(value->v.v_closure);
      break;
  }

  return nvalue;
}

/* css value node */
CstCssValueNode* cst_css_value_node_lookup(const SysChar *name) {
  sys_return_val_if_fail(name != NULL, 0);

  return (CstCssValueNode*)sys_hash_table_lookup(gcss_node_ht, (SysPointer)name);
}

const SysChar *cst_css_value_node_name(CstCssValueNode *node) {
  sys_return_val_if_fail(node != NULL, NULL);

  return node->name;
}

/* css basic api */
static SysInt cst_css_set_x(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_INT, SYS_FAILED);
  CstNode *parent = cst_node_parent(node);
  
  const FRRect *bound = cst_node_get_bound(parent);

  if (cst_node_is_abs_node(node)) {

    cst_node_set_x(node, v->v.v_int);
  } else {

    if (parent) {
      cst_node_set_x(node, bound->x + v->v.v_int);
    }
  }

  return SYS_SUCCESS;
}

static SysInt cst_css_set_y(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_INT, SYS_FAILED);
  CstNode *parent = cst_node_parent(node);

  const FRRect *bound = cst_node_get_bound(parent);

  if (cst_node_is_abs_node(node)) {

    cst_node_set_y(node, v->v.v_int);
  } else {
    if (parent) {

      cst_node_set_y(node, bound->y + v->v.v_int);
    }
  }

  return SYS_SUCCESS;
}

static SysInt cst_css_set_padding(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_M4, SYS_FAILED);

  cst_node_set_padding(node, v->v.v_m4);

  return SYS_SUCCESS;
}

static SysInt cst_css_set_margin(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_M4, SYS_FAILED);
  sys_return_val_if_fail(v->v.v_m4 != NULL, SYS_FAILED);

  cst_node_set_margin(node, v->v.v_m4);

  return SYS_SUCCESS;
}

void cst_css_value_width_percent(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer value) {
  sys_return_if_fail(v_node != NULL);
  sys_return_if_fail(cst_node_parent(v_node) != NULL);
  sys_return_if_fail(value != NULL);

  SysInt pwidth = 0;
  FRSInt4 m4;
  SysInt64 d = POINTER_TO_INT(value);

  pwidth = cst_node_get_width(v_parent);

  cst_node_get_mbp(v_node, &m4);

  pwidth = pwidth - m4.m1 - m4.m3;

  cst_node_set_width(v_node, (SysInt)(pwidth * d * 0.01));
}

void cst_css_value_height_percent(CstNode *v_parent, CstNode *v_node, FRContext *cr, SysPointer value) {
  sys_return_if_fail(v_node != NULL);
  sys_return_if_fail(v_parent != NULL);
  sys_return_if_fail(value != NULL);

  FRSInt4 m4;
  SysInt64 d = POINTER_TO_INT(value);

  SysInt pheight = 0;

  pheight = cst_node_get_height(v_parent);

  cst_node_get_mbp(v_node, &m4);

  pheight = pheight - m4.m0 - m4.m2;

  cst_node_set_height(v_node, (SysInt)(pheight * d * 0.01));
}

CstCssClosure* cst_css_value_parse_calc(SysChar *s, CstCssCalcFunc func) {
  SysSize slen = sys_strlen(s, 5);
  SysInt64 r = 0;

  if (*(s + (slen - 1)) == '%') {
    *(s + (slen - 1)) = '\0';

     r = sys_str_to_int64(s);

  } else {

    return NULL;
  }

  return cst_css_closure_new(INT_TO_POINTER(r), func, NULL);
}

static SysInt cst_css_set_width(CstNode *node, FRContext *cr, CstCssValue *v) {
  if (v->d_type == CST_CSS_VALUE_INT) {
    cst_node_set_width(node, v->v.v_int);

  } else if (v->d_type == CST_CSS_VALUE_CLOSURE) {
    cst_css_closure_ref(v->v.v_closure);

    cst_node_set_width_closure(node, v->v.v_closure);
  } else {
    sys_warning_N("Failed to set css width with type: %d", v->d_type);
    return SYS_FAILED;
  }

  return SYS_SUCCESS;
}

static SysInt cst_css_set_height(CstNode *node, FRContext *cr, CstCssValue *v) {
  if (v->d_type == CST_CSS_VALUE_INT) {
    cst_node_set_height(node, v->v.v_int);
  }
  else if (v->d_type == CST_CSS_VALUE_CLOSURE) {
    cst_css_closure_ref(v->v.v_closure);

    cst_node_set_height_closure(node, v->v.v_closure);

  } else {

    sys_warning_N("Failed to set css height with type: %d", v->d_type);
    return SYS_FAILED;
  }

  return SYS_SUCCESS;
}

static SysInt cst_css_set_font_size(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(node != NULL, SYS_FAILED);
  sys_return_val_if_fail(v != NULL, SYS_FAILED);

  CstText *text = CST_TEXT(node);
  sys_return_val_if_fail(node != NULL, SYS_FAILED);
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_INT, SYS_FAILED);

  if (!sys_object_is_a(node, CST_TYPE_TEXT)) {
    sys_warning_N("\"%s\" is not a Text type", cst_node_get_id(node));
    return SYS_FAILED;
  }

  cst_text_set_font_size(text, v->v.v_int);

  return SYS_SUCCESS;
}

static SysInt cst_css_set_wrap(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(node != NULL, SYS_FAILED);
  sys_return_val_if_fail(v != NULL, SYS_FAILED);
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_BOOL, SYS_FAILED);

  cst_node_set_wrap(node, v->v.v_bool);

  return SYS_SUCCESS;
}

static SysInt cst_css_set_font_family(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(node != NULL, SYS_FAILED);
  sys_return_val_if_fail(v != NULL, SYS_FAILED);

  CstText *text = CST_TEXT(node);
  sys_return_val_if_fail(node != NULL, SYS_FAILED);
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_STRING, SYS_FAILED);

  sys_return_val_if_fail(sys_object_is_a(node, CST_TYPE_TEXT), SYS_FAILED);
  cst_text_set_font_desc(text, v->v.v_string);

  return SYS_SUCCESS;
}

static SysInt cst_css_render_color(CstNode *node, FRContext *cr, CstCssValue *v) {
  sys_return_val_if_fail(node != NULL, SYS_FAILED);
  sys_return_val_if_fail(v->d_type == CST_CSS_VALUE_COLOR, SYS_FAILED);

  FRColor *color = v->v.v_color;

  fr_context_set_source_rgba(cr, color->r, color->g, color->b, color->a);

  return SYS_SUCCESS;
}

static  CstCssValueClass  x_class            =  {  &x_node,            ast_css_value_parse,         cst_css_set_x,            NULL                         };
static  CstCssValueClass  y_class            =  {  &y_node,            ast_css_value_parse,         cst_css_set_y,            NULL                         };
static  CstCssValueClass  padding_class      =  {  &padding_node,      ast_css_value_parse,         cst_css_set_padding,      NULL                         };
static  CstCssValueClass  margin_class       =  {  &margin_node,       ast_css_value_parse,         cst_css_set_margin,       NULL                         };
static  CstCssValueClass  width_class        =  {  &width_node,        ast_css_value_width_parse,   cst_css_set_width,        NULL                         };
static  CstCssValueClass  height_class       =  {  &height_node,       ast_css_value_height_parse,  cst_css_set_height,       NULL                         };
static  CstCssValueClass  color_class        =  {  &color_node,        ast_css_value_color_parse,   NULL,                     cst_css_render_color         };
static  CstCssValueClass  font_family_class  =  {  &font_family_node,  ast_css_value_parse,         cst_css_set_font_family,  NULL                         };
static  CstCssValueClass  font_size_class    =  {  &font_size_node,    ast_css_value_parse,         cst_css_set_font_size,    NULL                         };
static  CstCssValueClass  wrap_class         =  {  &wrap_node,         ast_css_value_parse,         cst_css_set_wrap,         NULL                         };

void cst_css_value_bind_class(const SysChar *name, CST_CSS_PROP_ENUM prop_type, CST_RENDER_STATE_ENUM g_type, CstCssValueClass *cls) {
  CstCssValueNode *node = cls->node;

  node->name = (SysChar *)name;
  node->cls_ptr = cls;
  node->p_type = prop_type;
  node->g_type = g_type;

  sys_hash_table_insert(gcss_node_ht, (SysPointer)name, (SysPointer)node);
}

void cst_css_value_setup(void) {
  // TODO: need refectoring later.
  sys_assert(gcss_node_ht == NULL && "css value should setup only once.");

  gcss_node_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, NULL);

  cst_css_value_bind_class("x",            CST_CSS_PROP_X,            CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &x_class            );
  cst_css_value_bind_class("y",            CST_CSS_PROP_Y,            CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &y_class            );
  cst_css_value_bind_class("padding",      CST_CSS_PROP_PADDING,      CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &padding_class      );
  cst_css_value_bind_class("margin",       CST_CSS_PROP_MARGIN,       CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &margin_class       );
  cst_css_value_bind_class("width",        CST_CSS_PROP_W,            CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &width_class        );
  cst_css_value_bind_class("height",       CST_CSS_PROP_H,            CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &height_class       );
  cst_css_value_bind_class("font-family",  CST_CSS_PROP_FONT_FAMILY,  CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &font_family_class  );
  cst_css_value_bind_class("font-size",    CST_CSS_PROP_FONT_SIZE,    CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &font_size_class    );
  cst_css_value_bind_class("wrap",         CST_CSS_PROP_WRAP,         CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT,  &wrap_class         );
  cst_css_value_bind_class("color",        CST_CSS_PROP_COLOR,        CST_RENDER_STATE_PAINT  | CST_RENDER_STATE_REPAINT,   &color_class        );
}

void cst_css_value_teardown(void) {
  sys_hash_table_unref(gcss_node_ht);
}

void cst_css_value_node_add(const SysChar* name, CstCssValueNode *node) {
  sys_hash_table_insert(gcss_node_ht, (SysPointer)cst_css_value_node_name(node), (SysPointer)node);
}
