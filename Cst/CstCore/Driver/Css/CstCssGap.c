#include <CstCore/Driver/Css/CstCssGap.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Parser/Ast.h>


SYS_DEFINE_TYPE(CstCssGap, cst_css_gap, CST_TYPE_CSS_VALUE);

static SysInt cst_css_set_value_i(CstCssValue *v, CstRenderNode *render_node, CstLayout *layout) {
  const FRRect *bound;
  CstRenderNode *pnode;

  bound = cst_render_node_get_bound(render_node);
  if (cst_render_node_is_abs_node(render_node)) {

    cst_render_node_set_x(render_node, v->v.v_int);
  } else {

    pnode = cst_render_node_get_parent(render_node);
    if (pnode) {

      cst_render_node_set_margin(render_node, v->v.v_m4);
    }
  }

  return SYS_SUCCESS;
}

/* object api */
static void cst_css_gap_dispose(SysObject* o) {
  CstCssGap *self = CST_CSS_GAP(o);

  SYS_OBJECT_CLASS(cst_css_gap_parent_class)->dispose(o);
}

static void cst_css_gap_construct(CstCssValue* o, SysChar *name, SysInt css_type, SysInt state_flag) {

  CST_CSS_VALUE_GET_CLASS(cst_css_gap_parent_class)->construct(o, name, css_type, state_flag);
}

CstCssValue *cst_css_gap_new_I(void) {
  CstCssValue *o = cst_css_gap_new();

  cst_css_gap_construct(o, "gap", CST_CSS_PROP_GAP, CST_RENDER_STATE_LAYOUT | CST_RENDER_STATE_RELAYOUT);

  return o;
}

CstCssValue *cst_css_gap_new(void) {
  return sys_object_new(CST_TYPE_CSS_GAP, NULL);
}

static void cst_css_gap_class_init(CstCssGapClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);
  CstCssValueClass *vcls = CST_CSS_VALUE_CLASS(cls);

  ocls->dispose = cst_css_gap_dispose;
  vcls->set_value = cst_css_set_value_i;
}

static void cst_css_gap_init(CstCssGap *self) {
}


