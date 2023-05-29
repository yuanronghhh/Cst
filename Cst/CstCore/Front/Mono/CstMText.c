#include <CstCore/Front/Mono/CstMText.h>


SysType cst_text_get_type(void) {
  static SysType meta = 0;

  if (meta != 0) {
    return meta;
  }

  FRMonoContext *c = cst_context_get_mono_context();
  meta = fr_mono_get_meta_from_name(c, "CstGUI.CstCore", "CstText");

  return meta;
}

CstNode* cst_text_new(void) {
  return fr_mono_object_new(CST_TYPE_TEXT);
}
