#ifndef __CST_NODE_H__
#define __CST_NODE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_NODE (cst_node_get_type())
#define CST_NODE(o) ((CstNode* )sys_object_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_CLASS(o) ((CstNodeClass *)sys_class_cast_check(o, CST_TYPE_NODE))
#define CST_NODE_GET_CLASS(o) sys_instance_get_class(o, CstNodeClass)

struct _CstNode {
  SysObject unowned;

  CstNode   *next;
  CstNode   *prev;
  CstNode   *parent;
  CstNode   *children;

  /* <private> */
  CstNode     *last_child;
  SysChar     *name;
  SysChar     *id;

  /* Type: FRAWatch */
  SysList *awatches;

  /* Type: CstNodeMap */
  SysList *node_maps;

  SysPtrArray* css_groups;

  /* CST_RENDER_NODE_ENUM  */
  SysInt position;
};

struct _CstNodeClass {
  SysObjectClass parent;

  void (*construct) (CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props);
  CstNode * (*dclone) (CstNode *node);
  void (*realize) (CstModule* v_module, CstComNode* com_node, CstNode* v_parent, CstNode* self, CstRender* v_render);
};

struct _CstNodeProps {
  SysList *v_awatches;
  SysList *v_node_maps;

  const SysChar* v_id;
  const SysChar *v_tag;
  SysChar **v_base;
  SysInt  v_base_len;
  SysInt v_position;
  SysChar *v_value;
  SysChar *v_label;
  SysInt  v_z_index;
  SysBool v_pass;
};

CstNode* cst_node_new(void);
SysType cst_node_get_type(void);

CstNode* cst_node_parent(CstNode *node);
CST_NODE_PROP_ENUM cst_node_prop_get_by_name(const SysChar *name);

void cst_node_unlink_node_r(CstNode *node);

CstNode* cst_node_insert_after(CstNode *parent, CstNode *sibling, CstNode *node);
void cst_node_append(CstNode *parent, CstNode *node);

void cst_node_print_node_r(CstNode* node);
void cst_node_print_node(CstNode *node);

void cst_node_set_name(CstNode *node, const SysChar *name);
const SysChar *cst_node_get_name(CstNode *node);

void cst_node_set_id(CstNode *node, const SysChar *id);
const SysChar *cst_node_get_id(CstNode *node);

void cst_node_realize(CstModule *v_module, CstComNode *ncomp_node, CstNode *v_parent, CstNode *v_node, CstRender *v_render);
CstNode *cst_node_children(CstNode *node);
CstNode *cst_node_prev(CstNode *node);
CstNode *cst_node_next(CstNode *node);
FRAWatch *cst_node_get_awatch(CstNode *node, SysType atype, const SysChar *bind_var);
CstNode *cst_node_get_last_child(CstNode *node);
void cst_node_set_last_child(CstNode *node, CstNode *last_child);
void cst_node_add_awatch(CstNode *node, FRAWatch *awatch);

void cst_node_realize_root(CstModule *v_module, CstComNode *ncomp_node, CstNode *root, CstNode *new_root, CstRender *v_render);

CstNode *cst_node_dclone(CstNode *v_node);
void cst_node_bind(CstNode *self, CstComNode *com_node);
void cst_node_construct(CstModule *v_module, CstComponent *v_component, CstNode *v_parent, CstNode *v_node, CstNodeProps *v_props);

SYS_END_DECLS

#endif
