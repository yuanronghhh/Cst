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

  CstNodePrivate* priv;
};

struct _CstNodeClass {
  SysObjectClass parent;

  void (*construct) (CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props);
 CstNode * (*dclone) (CstNode *node);
 CstNode *(*realize) (CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render);
 void (*relayout) (CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
 void (*relayout_down) (CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, FRContext *cr);
 void (*repaint) (CstModule *v_module, CstNode *v_parent, CstNode *v_node, FRContext *cr, FRDraw *draw, SysInt state);
};


SYS_END_DECLS

#endif

