#ifndef __CST_CCOMPONENT__
#define __CST_CCOMPONENT__

#include <CstCore/Front/Common/CstComponentPrivate.h>


SYS_BEGIN_DECLS

struct _CstComponent {
  FREnv parent;
  
  /* <private> */
  CstModule *v_module;
  SysChar *id;
  /* ComStyle : CstCssGroup */
  FREnv *style_env;
  /* ComProps: CstPropMap */
  FREnv *prop_maps_env;
  CstNode *layout_node;
};

struct _CstComponentClass {
  FREnvClass parent;

  void (*construct) (CstComponent *self, CstModule *v_module, CstComponent *v_parent);
};

SYS_END_DECLS

#endif

