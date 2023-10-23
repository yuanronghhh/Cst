#include <CstCore/Parser/Ast.h>
#include <CstCore/Front/CstFrontCore.h>

#include <CstCore/Driver/Css/CstCssValue.h>
#include <CstCore/Driver/Css/CstCssGroup.h>
#include <CstCore/Driver/Css/CstCssPair.h>
#include <CstCore/Driver/Css/CstCssEnv.h>
#include <CstCore/Driver/Css/CstCssParser.h>
#include <CstCore/Driver/Css/CstCssNode.h>

#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstComponentBuilder.h>
#include <CstCore/Driver/CstNodeBuilder.h>
#include <CstCore/Driver/CstPropPair.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstManager.h>


typedef struct _AstModulePass AstModulePass;
typedef struct _AstComponentPass AstComponentPass;
typedef struct _AstGStylePass AstGStylePass;
typedef struct _AstNodePass AstNodePass;
typedef struct _AstKeyNode AstKeyNode;


struct _AstKeyNode {
  SysChar* key;
  SysPointer value;
};

struct _Source {
  SysChar* string;
  SysInt startline;
};

struct _JPair {
  SysChar* key;
  JNode* prop;
  JNode* value;
};

struct _JObject {
  JPair** pairs;
  SysInt len;
};

struct _JNode {
  JNodeType type;

  union {
    JNode* v_node;
    JSource* v_source;
    JPointer v_pointer;
    JBool v_bool;
    JDouble v_double;
    JInt v_int;
    JChar* v_string;
    JPair* v_pair;
    JObject* v_object;
    JArray* v_array;
  } v;
};

struct _Component {
  JNode *property;
  JNode *body;
};

struct _GStyle {
  JNode *property;
  JNode *body;
};

struct _Import {
  SysPtrArray *ids;
  SysChar *path;
};

struct _AstNode {
  AstNodeType type;

  union {
    SysPtrArray *root;
    Import *import;
    Source *source;
    Component *component;
    GStyle *gstyle;
    JNode *jnode;
  } v;
};

struct _AstNodePass {
  CstNode *v_com_node;
  CstNodeBuilder *v_node_builder;
};

struct _AstGStylePass {
  const SysChar *path;
  CstCssEnv *gcss_env;
};

struct _AstComponentPass {
  CstComponent  *v_component;
  CstNode *v_pnode;
  CstComponentBuilder *v_comp_builder;
};

struct _AstModulePass {
  CstModule  *v_module;
  CstCssEnv* gcss_env;
};
