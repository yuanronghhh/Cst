#include <CstCore/Front/Mono/CstMLBody.h>


SysType cst_lbody_get_type(void) {
  static SysType meta = 0;

  if (meta != 0) {
    return meta;
  }

  FRMonoContext *c = _cst_context_get_mono_context();
  meta = fr_mono_get_meta_from_name(c, "CstGUI.CstCore", "CstText");

  return meta;
}

CstNode* cst_lbody_new(void) {
  return fr_mono_object_new(CST_TYPE_LBODY);
}
