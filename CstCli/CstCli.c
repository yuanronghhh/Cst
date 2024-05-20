#include "CstCli.h"

int main(int argc, char* argv[]) {
  CstModule *mod;

  cst_core_setup();

  SysChar *entry = CST_PROJECT_DIR"/Cst/Mini/Front/MiniComponent.cst";

  mod = cst_module_load_path(NULL, entry);

  sys_object_unref(mod);

  cst_core_teardown();

  return 0;
}
