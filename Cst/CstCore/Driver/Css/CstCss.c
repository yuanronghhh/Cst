#include <CstCore/Driver/Css/CstCss.h>

#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Driver/Css/CstCssEnv.h>

void cst_css_setup(void) {
  cst_css_pair_setup();
  cst_css_env_setup();
}

void cst_css_teardown(void) {
  cst_css_pair_teardown();
  cst_css_env_teardown();
}
