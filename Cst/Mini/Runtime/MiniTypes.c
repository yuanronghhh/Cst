/**
 * MiniTypes auto generated from TypesCode.code
 */

#include <Mini/Front/MiniComponent.h>
#include <Mini/Front/MiniMenuBar.h>
#include <Mini/Front/MiniMenuBase.h>
#include <Mini/Runtime/MiniTypes.h>


void mini_types_setup(CstApplication *app) {
  cst_application_set_meta(app, "mini-menubase", MINI_TYPE_MENUBASE);
  cst_application_set_meta(app, "mini-menubar", MINI_TYPE_MENUBAR);
  cst_application_set_meta(app, "mini-component", MINI_TYPE_COMPONENT);
}
