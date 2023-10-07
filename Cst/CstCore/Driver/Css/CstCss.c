#include <CstCore/Driver/Css/CstCss.h>
#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssEnv.h>


void cst_css_setup(void) {
  cst_css_value_setup();
  cst_css_env_setup();
}

void cst_css_teardown(void) {
  cst_css_value_teardown();
  cst_css_env_teardown();
}
