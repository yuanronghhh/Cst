#include <CstCore/Front/Common/CstComponentPrivate.h>


SysType _cst_component_get_type(void) {
  static SysType meta = 0;

  if (meta != 0) {
    return meta;
  }

  FRMonoContext *c = cst_context_get_mono_context();
  meta = fr_mono_get_meta_from_name(c, "CstGUI.CstCore", "CstComponent");

  return meta;
}

CstNode* cst_component_new(void) {
  return fr_mono_object_new(CST_TYPE_COMPONENT);
}

const SysChar* _cst_component_get_id(CstComponent* self) {
  return NULL;
}

void _cst_component_set_id(CstComponent* self, const SysChar *id) {

}

void _cst_component_layout_count_inc(CstComponent* self) {

}

SysInt _cst_component_layout_count(CstComponent* self) {
  return NULL;
}

CstCssGroup *_cst_component_get_css_r(CstComponent* self, const SysChar *key) {
  return NULL;
}

CstCssGroup *_cst_component_get_css(CstComponent* self, const SysChar *key) {
  return NULL;
}

void _cst_component_set_css(CstComponent* self, CstCssGroup *g) {

}

SysBool _cst_component_remove_css(CstComponent * self, CstCssGroup * g) {
  return NULL;
}

CST_COMPONENT_BTYPE _cst_component_body_type(const SysChar* name) {
  return NULL;
}

CstCssEnv* _cst_component_get_css_env(CstComponent *self) {
  return NULL;
}

void _cst_component_realize(CstComponent *self, CstRender *render, CstNode *pnode) {

}

SysBool _component_realize_jpair(JNode *node, SysPointer param_ptr) {
  return NULL;
}

SysBool _cst_component_style_node_callback(JNode *node, SysPointer self_ptr) {
  return NULL;
}
