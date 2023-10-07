#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssValueX.h>
#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/Css/CstCssClosure.h>
#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Driver/CstLayout.h>



SYS_DEFINE_TYPE(CstCssValue, cst_css_value, SYS_TYPE_OBJECT);

static SysHashTable *gcss_node_ht = NULL;

SysInt cst_css_value_get_state(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, -1);

  return self->state_flag;
}

CstCssValue *cst_css_value_dclone(CstCssValue *self) {
  CstCssValueClass *cls = CST_CSS_VALUE_GET_CLASS(self);

  sys_return_val_if_fail(cls->dclone != NULL, NULL);
  return cls->dclone(self);
}

void cst_css_value_set_value(CstCssValue *self, CstRenderNode *render_node, CstLayout *layout) {
  CstCssValueClass *cls = CST_CSS_VALUE_GET_CLASS(self);

  sys_return_if_fail(cls->dclone != NULL);
  cls->set_value(self, render_node, layout);
}

void cst_css_value_calc(CstCssValue *self, CstRenderNode *render_node, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  CstCssValueClass *cls = CST_CSS_VALUE_GET_CLASS(self);
  int state = cst_layout_get_state(layout);
  FRContext *cr = cst_layout_get_cr(layout);

  if (!(state & self->state_flag)) {
    return;
  }

  switch (state) {
    case CST_RENDER_STATE_LAYOUT:
    case CST_RENDER_STATE_RELAYOUT:

      cls->calc(self, render_node, layout);
      break;
    case CST_RENDER_STATE_PAINT:
    case CST_RENDER_STATE_REPAINT:
      break;

    default:
      sys_warning_N("Not support css render state: %d.", self->state_flag);
      break;
  }
}

SysInt cst_css_value_parse(JNode *jnode, CstCssValue *value) {
  sys_return_val_if_fail(value != NULL, -1);

  CstCssValueClass *cls = CST_CSS_VALUE_GET_CLASS(value);
  sys_return_val_if_fail(cls->parse != NULL, -1);

  return cls->parse(jnode, value);
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

const SysChar *cst_css_value_node_name(CstCssValue *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return self->name;
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

static void cst_css_value_bind_map(const SysChar *name, SysType type) {
  sys_hash_table_insert(gcss_node_ht, (SysPointer)name, (SysPointer)type);
}

void cst_css_value_setup(void) {
  sys_assert(gcss_node_ht == NULL && "css value should setup only once.");

  gcss_node_ht = sys_hash_table_new_full(sys_str_hash, (SysEqualFunc)sys_str_equal, NULL, NULL);

  cst_css_value_bind_map("x", CST_TYPE_CSS_VALUE_X);
  // cst_css_value_bind_map("y",           CST_CSS_TYPE_Y           );
  // cst_css_value_bind_map("padding",     CST_CSS_TYPE_PADDING     );
  // cst_css_value_bind_map("margin",      CST_CSS_TYPE_MARGIN      );
  // cst_css_value_bind_map("width",       CST_CSS_TYPE_W           );
  // cst_css_value_bind_map("height",      CST_CSS_TYPE_H           );
  // cst_css_value_bind_map("font-family", CST_CSS_TYPE_FONT_FAMILY );
  // cst_css_value_bind_map("font-size",   CST_CSS_TYPE_FONT_SIZE   );
  // cst_css_value_bind_map("wrap",        CST_CSS_TYPE_WRAP        );
  // cst_css_value_bind_map("color",       CST_CSS_TYPE_COLOR       );
}

void cst_css_value_teardown(void) {
  sys_assert(gcss_node_ht != NULL && "css value should init before use.");

  sys_hash_table_unref(gcss_node_ht);
}

SysType cst_css_value_node_lookup(const SysChar *name) {
  return (SysType)sys_hash_table_lookup(gcss_node_ht, (const SysPointer)name);
}

/* object api */
static void cst_css_value_dispose(SysObject* o) {
  CstCssValue *self = CST_CSS_VALUE(o);

  SYS_OBJECT_CLASS(cst_css_value_parent_class)->dispose(o);
}

static void cst_css_value_construct(CstCssValue* self) {
}

CstCssValue *cst_css_value_new(void) {
  return sys_object_new(CST_TYPE_CSS_VALUE, NULL);
}

static void cst_css_value_class_init(CstCssValueClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_css_value_dispose;

  cls->parse = ast_css_value_parse;
}

static void cst_css_value_init(CstCssValue *self) {
}
