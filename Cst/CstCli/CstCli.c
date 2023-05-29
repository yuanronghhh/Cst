#include "CstCli.h"

int main(int argc, char* argv[]) {
  UNUSED(argc);
  UNUSED(argv);

  CstModule *mod;

  cst_application_env_setup();
  CstManager *manager = cst_manager_new();

  SysChar *entry = CST_PROJECT_DIR"/Cst/Mini/Front/MiniComponent.cst";

  mod = cst_module_new_I(manager, NULL, entry);
  cst_manager_gencode(manager, mod);

  cst_application_env_teardown();

  sys_leaks_report();
  return 0;
}
