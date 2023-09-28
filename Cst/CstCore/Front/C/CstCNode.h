#ifndef __CST_CNODE_H__
#define __CST_CNODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

struct _CstNode {
  SysObject unowned;

  CstNode   *next;
  CstNode   *prev;
  CstNode   *parent;
  CstNode   *children;

  /* <private> */
  SysChar     *name;
  CstNode     *last_child;
  SysChar     *id;
  SysPtrArray *css_groups;
  /* Type: FRAWatch */
  SysList *awatches;
  /* Type: CstNodeMap */
  SysList *node_maps;
  SysList *pass_link;
  FRPrioLink *abs_link;
  SysList *shadow_link;

  /* layout context */
  CstPaintNode *paint_node;
  CstLayoutNode *layout_node;
  CstLayoutContext *layout_ctx;
};

struct _CstNodeClass {
  SysObjectClass parent;

  void (*construct) (CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props);
  CstNode * (*dclone) (CstNode *node);
  CstNode *(*realize) (CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render);
};


SYS_END_DECLS

#endif

