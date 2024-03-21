#ifndef __CST_TYPES_H__
#define __CST_TYPES_H__

#include <CstCore/CstConfig.h>
#include <Framework/FRCore.h>


SYS_BEGIN_DECLS

typedef enum _CST_CSS_PROP_ENUM {
  CST_CSS_PROP_X,
  CST_CSS_PROP_Y,
  CST_CSS_PROP_W,
  CST_CSS_PROP_H,
  CST_CSS_PROP_LAYER,
  CST_CSS_PROP_MARGIN,
  CST_CSS_PROP_BORDER,
  CST_CSS_PROP_PADDING,
  CST_CSS_PROP_GAP,
  CST_CSS_PROP_FONT_SIZE,
  CST_CSS_PROP_FONT_FAMILY,
  CST_CSS_PROP_FRONT,

  CST_CSS_PROP_WRAP,
  CST_CSS_PROP_COLOR,
  CST_CSS_PROP_BACGROUND,
  CST_CSS_PROP_BORDERRADIUS,
} CST_CSS_PROP_ENUM;

typedef enum _CST_CSS_VALUE_ENUM {
  CST_CSS_VALUE_BOOL = 1,
  CST_CSS_VALUE_NULL,
  CST_CSS_VALUE_DOUBLE,
  CST_CSS_VALUE_INT,
  CST_CSS_VALUE_STRING,
  CST_CSS_VALUE_POINTER,
  CST_CSS_VALUE_M4,        // store value with length 4.
  CST_CSS_VALUE_COLOR,
  CST_CSS_VALUE_CLOSURE,
} CST_CSS_VALUE_ENUM;

typedef enum _CST_DIRECTION_ENUM {
  CST_DIRECTION_HORIZONTAL,
  CST_DIRECTION_HORIZONTAL_REVERSE,
  CST_DIRECTION_VERTICAL,
  CST_DIRECTION_VERTICAL_REVERSE,
} CST_DIRECTION_ENUM;

typedef enum _CST_AWATCH_ENUM {
  CST_AWATCH_ACTION,
  CST_AWATCH_CSS_VALUE,
  CST_AWATCH_FUNCTION
} CST_AWATCH_ENUM;

typedef enum _CST_COMPONENT_BODY_ENUM {
  CST_COMPONENT_BDATA,
  CST_COMPONENT_BCOMPONENT,
  CST_COMPONENT_BLAYOUT,
  CST_COMPONENT_BSTYLE,
  CST_COMPONENT_BPROPS,
  CST_COMPONENT_BLAST
} CST_COMPONENT_BODY_ENUM;

typedef enum _CST_COMPONENT_PROP_ENUM {
  CST_COMPONENT_PROP_ID,
  CST_COMPONENT_PROP_BASE,
} CST_COMPONENT_PROP_ENUM;

typedef enum _CST_NODE_PROP_ENUM {
  CST_NODE_PROP_ID,
  CST_NODE_PROP_VALUE,
  CST_NODE_PROP_BASE,
  CST_NODE_PROP_POSITION,
  CST_NODE_PROP_LABEL,
  CST_NODE_PROP_BIND,
  CST_NODE_PROP_ACTION,
} CST_NODE_PROP_ENUM;

typedef enum _CST_RENDER_STATE_ENUM {
  CST_RENDER_STATE_LAYOUT = 1 << 0,
  CST_RENDER_STATE_PAINT = 1 << 1,
  CST_RENDER_STATE_RELAYOUT = 1 << 2,
  CST_RENDER_STATE_REPAINT = 1 << 3,
} CST_RENDER_STATE_ENUM;

typedef enum _CST_DIRTY_STATE_ENUM {
  CST_DIRTY_RELAYOUT = 1 << 0,
  CST_DIRTY_REREPAINT = 1 << 1,
  CST_DIRTY_DIRTY = CST_DIRTY_RELAYOUT | CST_DIRTY_REREPAINT,
  CST_DIRTY_MASK = CST_DIRTY_DIRTY,
} CST_DIRTY_STATE_ENUM;

typedef enum _CST_NODE_POSITION_ENUM {
  CST_NODE_POSITION_RELATIVE = 1 << 0,
  CST_NODE_POSITION_ABSOLUTE = 1 << 1,
  CST_NODE_POSITION_FIXED = 1 << 2,
  CST_NODE_POSITION_STATIC = 1 << 3,
} CST_NODE_POSITION_ENUM;

typedef enum _CST_NODE_LAYER_ENUM {
  CST_NODE_LAYER_BOX = 1 << 1,
  CST_NODE_LAYER_ABS = 1 << 2,
  CST_NODE_LAYER_STATIC = 1 << 3,
  CST_NODE_LAYER_PASS = 1 << 4,
  CST_NODE_LAYER_MASK = (1 << 5) - 1
} CST_NODE_LAYER_ENUM;

typedef enum _FLEX_DIRECTION_ENUM {
  FLEX_DIRECTION_ROW,
  FLEX_DIRECTION_ROW_REVERSE,
  FLEX_DIRECTION_COLUMN,
  FLEX_DIRECTION_COLUMN_REVERSE
} FLEX_DIRECTION_ENUM;

typedef struct _CstContext CstContext;

typedef struct _CstApplication CstApplication;
typedef struct _CstApplicationClass CstApplicationClass;

typedef struct _CstManager CstManager;
typedef struct _CstManagerClass CstManagerClass;

typedef struct _CstModuleContext CstModuleContext;
typedef struct _CstModule CstModule;
typedef struct _CstModuleClass CstModuleClass;

typedef struct _CstRender CstRender;
typedef struct _CstRenderClass CstRenderClass;

typedef struct _CstILayerNode CstILayerNode;
typedef struct _CstILayerNodeInterface CstILayerNodeInterface;

typedef struct _CstLayerNode CstLayerNode;
typedef struct _CstLayerNodeClass CstLayerNodeClass;

typedef struct _CstILayer CstILayer;
typedef struct _CstILayerInterface CstILayerInterface;

typedef struct _CstLayer CstLayer;
typedef struct _CstLayerClass CstLayerClass;

typedef struct _CstBoxLayer CstBoxLayer;
typedef struct _CstBoxLayerClass CstBoxLayerClass;

typedef struct _CstAbsLayer CstAbsLayer;
typedef struct _CstAbsLayerClass CstAbsLayerClass;

typedef struct _CstCssGroup CstCssGroup;
typedef struct _CstCssGroupClass CstCssGroupClass;

typedef struct _CstDrawNode CstDrawNode;
typedef struct _CstDrawNodeClass CstDrawNodeClass;

typedef struct _CstAlgorithm CstAlgorithm;
typedef struct _CstAlgorithmClass CstAlgorithmClass;

typedef struct _CstLayoutNode CstLayoutNode;
typedef struct _CstLayoutNodeClass CstLayoutNodeClass;
typedef struct _CstRenderContext CstRenderContext;
typedef struct _CstRenderContextClass CstRenderContextClass;

typedef struct _CstFlexContext CstFlexContext;
typedef struct _CstFlexContextClass CstFlexContextClass;

typedef struct _CstFlexItem CstFlexItem;
typedef struct _CstFlexItemInterface CstFlexItemInterface;

typedef struct _CstFlexAlgorithm CstFlexAlgorithm;
typedef struct _CstFlexAlgorithmClass CstFlexAlgorithmClass;
typedef struct _CstFlexLine CstFlexLine;
typedef struct _CstFlexLineClass CstFlexLineClass;
typedef struct _CstLayoutFlex CstLayoutFlex;
typedef struct _CstLayoutFlexClass CstLayoutFlexClass;

typedef struct _CstLayout CstLayout;
typedef struct _CstLayoutClass CstLayoutClass;

typedef struct _CstSurface CstSurface;
typedef struct _CstSurfaceClass CstSurfaceClass;

typedef struct _CstRenderNode CstRenderNode;
typedef struct _CstRenderNodeClass CstRenderNodeClass;
typedef struct _CstBoxNode CstBoxNode;
typedef struct _CstBoxNodeClass CstBoxNodeClass;

typedef struct _CstAbsNode CstAbsNode;
typedef struct _CstAbsNodeClass CstAbsNodeClass;

typedef struct _CstCssClosure CstCssClosure;
typedef struct _CstCssClosureClass CstCssClosureClass;

typedef struct _CstCssParser CstCssParser;
typedef struct _CstCssParserClass CstCssParserClass;

typedef union _CstCssValueV CstCssValueV;
typedef struct _CstCssValue CstCssValue;
typedef struct _CstCssValueClass CstCssValueClass;

typedef struct _CstComponentContext CstComponentContext;
typedef struct _CstComponentContextClass CstComponentContextClass;

typedef struct _CstComponent CstComponent;
typedef struct _CstComponentClass CstComponentClass;

typedef struct _CstRenderContext CstRenderContext;
typedef struct _CstRenderContextClass CstRenderContextClass;

typedef struct _CstNodeBuilder CstNodeBuilder;
typedef struct _CstNodeBuilderClass CstNodeBuilderClass;

typedef struct _CstNode CstNode;
typedef struct _CstNodeClass CstNodeClass;

typedef struct _CstCssNode CstCssNode;
typedef struct _CstCssNodeClass CstCssNodeClass;
typedef struct _CstCssPair CstCssPair;
typedef struct _CstCssPairClass CstCssPairClass;

#include <CstCore/Front/CstType.h>

typedef struct _CstNodeMap CstNodeMap;
typedef struct _CstNodeMapClass CstNodeMapClass;

typedef struct _CstValueMap CstValueMap;
typedef struct _CstValueMapClass CstValueMapClass;

typedef void (*CstNodeMapFunc) (CstNode *node, CstNodeMap *map);
typedef void (*CstRNodeMapFunc) (CstRenderNode *rnode, CstNodeMap *map);
typedef void (*CstCssFree) (CstCssValue *value);
typedef SysBool (*CstLayerNodeFunc) (CstLayerNode *lnode, SysPointer user_data);
typedef void (*CstRenderNodeFunc) (CstRenderNode *rnode, SysPointer user_data);
typedef void (*CstCssFunc) (CstRenderNode *rnode, CstLayout *layout, SysPointer user_data);
typedef void (*CstNodeRealizeFunc) (CstNode *self, CstLayerNode* parent, CstModule *v_module, CstComNode *v_com_node, CstLayout *layout);

SYS_END_DECLS

#endif

