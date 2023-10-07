#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Front/Common/CstNode.h>
#include <CstCore/Front/Common/CstComponent.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstLayoutContext.h>
#include <CstCore/Driver/CstLayout.h>


SYS_DEFINE_TYPE(CstRenderNode, cst_render_node, SYS_TYPE_OBJECT);

void cst_render_node_set_layout(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_layout_context_set_layout(self->layout_ctx, bvalue);
}

SysBool cst_render_node_need_layout(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_need_layout(self->layout_ctx);
}

void cst_render_node_set_paint(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_layout_context_set_paint(self->layout_ctx, bvalue);
}

SysBool cst_render_node_need_paint(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_need_paint(self->layout_ctx);
}

void cst_render_node_set_abs_node(CstRenderNode *self, SysBool bvalue) {
  sys_return_if_fail(self != NULL);

  cst_layout_context_set_abs_node(self->layout_ctx, bvalue);
}

SysBool cst_render_node_is_abs_node(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_is_abs_node(self->layout_ctx);
}

const FRRect *cst_render_node_get_bound(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_layout_node_get_bound(self->layout_node);
}

void cst_render_node_set_x(CstRenderNode* self, SysInt x) {

  cst_layout_node_set_x(self->layout_node, x);
}

void cst_render_node_set_width(CstRenderNode* self, SysInt width) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_width(self->layout_node, width);
}

void cst_render_node_set_height(CstRenderNode* self, SysInt height) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_set_height(self->layout_node, height);
}

void cst_render_node_prepare(CstRenderNode *self) {
  sys_return_if_fail(self != NULL);

  FRSInt4 m4 = { 0 };

  cst_layout_node_get_mbp(self->layout_node, &m4);
  cst_layout_context_set_mbp(self->layout_ctx, &m4);
  cst_layout_context_calc_size(self->layout_ctx, self);
}

void cst_render_node_get_mbp(CstRenderNode* self, FRSInt4* m4) {
  sys_return_if_fail(self != NULL);

  cst_layout_node_get_mbp(self->layout_node, m4);
}


SysBool cst_render_node_is_dirty(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_need_layout(self->layout_ctx)
    || cst_layout_context_need_paint(self->layout_ctx);
}

void cst_render_node_mark_dirty(CstRenderNode* self, SysBool bvalue) {
}

SysBool cst_render_node_is_visible(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_is_visible(self->layout_ctx);
}

SysBool cst_render_node_can_wrap(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, false);

  return cst_layout_context_can_wrap(self->layout_ctx);
}

CstRenderNode* cst_render_node_get_parent(CstRenderNode* self) {
  sys_return_val_if_fail(self != NULL, NULL);

  CstRenderNodeClass *cls = CST_RENDER_NODE_GET_CLASS(self);
  sys_return_val_if_fail(cls->get_parent == NULL, NULL);

  return cls->get_parent(self);
}

void cst_render_node_render_enter(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);

  cst_css_render_groups(self->css_groups, self, layout);
  cst_render_node_prepare(self);
}

void cst_render_node_render_leave(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
}

CstRenderNode* cst_render_node_dclone_i(CstRenderNode *o) {
  sys_return_val_if_fail(o != NULL, NULL);
  SysType type = sys_type_from_instance(o);

  SysPtrArray *ptr;
  CstRenderNode *n = sys_object_new(type, NULL);

  ptr = n->css_groups;
  for(SysUInt i = 0; i < ptr->len; i++) {
    CstCssGroup *g = cst_css_group_clone(ptr->pdata[i]);

    sys_ptr_array_add(n->css_groups, g);
  }

  n->node = cst_node_dclone(o->node);
  n->layout_node = cst_layout_node_clone(o->layout_node);
  n->layout_ctx = cst_layout_context_dclone(o->layout_ctx);

  return n;
}

void cst_render_node_relayout_self(CstRenderNode *self, CstLayout *layout) {

  cst_layout_context_layout_self(self->layout_ctx, self, layout);
}

const SysChar* cst_render_node_get_id(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_node_get_id(self->node);
}

const SysChar* cst_render_node_get_name(CstRenderNode *self) {
  sys_return_val_if_fail(self != NULL, NULL);

  return cst_node_get_name(self->node);
}

void cst_render_node_paint_self(CstRenderNode *self, CstLayout *layout) {
  sys_return_if_fail(self != NULL);
}

/* css */
static SysBool render_node_css_exists(SysPtrArray *css_list, const SysChar *node_id, CstCssGroup *g) {
  sys_return_val_if_fail(css_list == NULL, false);
  sys_return_val_if_fail(node_id == NULL, false);

  if (css_list->len == 0) { return false; }

  for (SysInt i = 0; i < (SysInt)css_list->len; i++) {
    CstCssGroup * cg = css_list->pdata[i];

    if (cg == g) {
      sys_warning_N("load duplicate css: %s, %s", node_id, cst_css_group_get_id(g));
      return true;
    }
  }

  return false;
}

static SysBool render_node_set_css_r_i(CstRenderNode *self, CstCssGroup *g) {
  const SysChar *id;
  SysPtrArray* base;

  sys_return_val_if_fail(g != NULL, false);

  id = cst_css_group_get_id(g);
  sys_return_val_if_fail(id != NULL, false);

  base = cst_css_group_get_base(g);
  if (render_node_css_exists(self->css_groups, cst_node_get_id(self->node), g)) {
    return false;
  }

  sys_object_ref(g);
  sys_ptr_array_add(self->css_groups, g);

  if (base == NULL || base->len == 0) {
    return true;
  }

  for(SysInt i = 0; i < (SysInt)base->len; i++) {
    CstCssGroup * ng = base->pdata[i];

    sys_assert(cst_css_group_get_id(ng) != NULL && "CstCssGroup id should not be null, maybe destroyed ?");

    render_node_set_css_r_i(self, ng);
  }

  return false;
}

SysBool cst_render_node_set_css_r(CstRenderNode *self, CstCssGroup *g) {
  return render_node_set_css_r_i(self, g);
}

SysBool cst_render_node_set_css_by_id(CstRenderNode *self, SysChar *id, CstComponent *comp) {
  sys_return_val_if_fail(id != NULL, false);
  sys_return_val_if_fail(self != NULL, false);

  CstCssGroup *g = cst_component_get_css_r(comp, id);
  if (g == NULL) {
    sys_error_N("Not found css: %s", id);
    return false;
  }

  return cst_render_node_set_css_r(self, g);
}

/* object api */
static void cst_render_node_dispose(SysObject* o) {
  CstRenderNode *self = CST_RENDER_NODE(o);

  SYS_OBJECT_CLASS(cst_render_node_parent_class)->dispose(o);
}

static void cst_render_node_construct(CstRenderNode* self, CstNode *node) {
  self->node = node;
  self->layout_node = cst_layout_node_new_I(0, 0, -1, 1);
  self->layout_ctx = cst_layout_context_new_I();

  sys_object_ref(SYS_OBJECT(node));
}

CstRenderNode *cst_render_node_new(void) {
  return sys_object_new(CST_TYPE_RENDER_NODE, NULL);
}

CstRenderNode *cst_render_node_new_I(CstNode *node) {
  CstRenderNode *o = cst_render_node_new();

  cst_render_node_construct(o, node);

  return o;
}

static void cst_render_node_class_init(CstRenderNodeClass* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = cst_render_node_dispose;

  cls->construct = cst_render_node_construct;
}

static void cst_render_node_init(CstRenderNode *self) {
}
