#ifndef __CST_TYPES_H__
#define __CST_TYPES_H__

#include <CstCore/CstConfig.h>
#include <Framework/FRCore.h>


SYS_BEGIN_DECLS

typedef enum _CST_CSS_PROP_ENUM {
  CST_CSS_PROP_X = 1,
  CST_CSS_PROP_Y,
  CST_CSS_PROP_W,
  CST_CSS_PROP_H,
  CST_CSS_PROP_PADDING,
  CST_CSS_PROP_MARGIN,
  CST_CSS_PROP_LEFT,
  CST_CSS_PROP_RIGHT,
  CST_CSS_PROP_BORDER,
  CST_CSS_PROP_WRAP,
  CST_CSS_PROP_FONT_SIZE,
  CST_CSS_PROP_FONT_FAMILY,
  CST_CSS_PROP_COLOR,
  CST_CSS_PROP_BACGROUND,
  CST_CSS_PROP_BORDERRADIUS,
  CST_CSS_PROP_LAST,
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
  CST_DIRECTION_VERTICAL,
  CST_DIRECTION_HORIZONTAL,
} CST_DIRECTION_ENUM;

typedef enum _CST_DIMENTION_ENUM {
  CST_DIMENTION_WIDTH,
  CST_DIMENTION_HEIGHT,
} CST_DIMENTION_ENUM;

typedef enum _CST_LAYER_ENUM {
  CST_LAYER_BOX = 1 << 0,
  CST_LAYER_STATIC = 1 << 1,
  CST_LAYER_ABS = 1 << 2,
  CST_LAYER_PASS = 1 << 3,
  CST_LAYER_MASK = (1 << 4) - 1
} CST_LAYER_ENUM;

typedef enum _CST_AWATCH_ENUM {
  CST_AWATCH_ACTION,
  CST_AWATCH_CSS_VALUE,
  CST_AWATCH_FUNCTION
} CST_AWATCH_ENUM;

typedef enum _FR_ACTION_ENUM CST_ACTION_ENUM;

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
  CST_NODE_PROP_ABSOLUTE,
  CST_NODE_PROP_LABEL,

  CST_NODE_PROP_ACTION,
  CST_NODE_PROP_LAST
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

typedef enum _CST_RENDER_NODE_ENUM {
  CST_RENDER_NODE_BOX = 1 << 0,
  CST_RENDER_NODE_ABS = 1 << 1,
  CST_RENDER_NODE_MASK = 0x03,
} CST_RENDER_NODE_ENUM;

typedef SYS_VALUE_ENUM CST_PROP_VALUE_ENUM;

typedef struct _CstContext CstContext;

typedef struct _CstApplication CstApplication;
typedef struct _CstApplicationClass CstApplicationClass;
typedef struct _CstApplicationPrivate CstApplicationPrivate;

typedef struct _CstManager CstManager;
typedef struct _CstManagerClass CstManagerClass;

typedef struct _CstModule CstModule;
typedef struct _CstModuleClass CstModuleClass;

typedef struct _CstRender CstRender;
typedef struct _CstRenderClass CstRenderClass;

typedef struct _CstLayer CstLayer;
typedef struct _CstLayerClass CstLayerClass;
typedef struct _CstBoxLayer CstBoxLayer;
typedef struct _CstBoxLayerClass CstBoxLayerClass;
typedef struct _CstAbsLayer CstAbsLayer;
typedef struct _CstAbsLayerClass CstAbsLayerClass;

typedef struct _CstCssGroup CstCssGroup;
typedef struct _CstCssGroupClass CstCssGroupClass;

typedef struct _CstLayoutNode CstLayoutNode;
typedef struct _CstLayoutNodeClass CstLayoutNodeClass;
typedef struct _CstLayoutContext CstLayoutContext;
typedef struct _CstLayoutContextClass CstLayoutContextClass;

typedef struct _CstLayout CstLayout;
typedef struct _CstLayoutClass CstLayoutClass;

typedef struct _CstRenderNode CstRenderNode;
typedef struct _CstRenderNodeClass CstRenderNodeClass;
typedef struct _CstBoxNode CstBoxNode;
typedef struct _CstBoxNodeClass CstBoxNodeClass;

typedef struct _CstCssClosure CstCssClosure;
typedef struct _CstCssClosureClass CstCssClosureClass;

typedef struct _CstCssValue CstCssValue;
typedef struct _CstCssValueClass CstCssValueClass;

typedef struct _CstCssValueX CstCssValueX;
typedef struct _CstCssValueXClass CstCssValueXClass;

typedef struct _CstCssPair CstCssPair;
typedef struct _CstCssPairClass CstCssPairClass;

typedef struct _CstCssEnv CstCssEnv;
typedef struct _CstCssEnvClass CstCssEnvClass;

#include <CstCore/Front/CstType.h>

typedef struct _CstNodeProps CstNodeProps;
typedef struct _CstComponentProps CstComponentProps;
typedef struct _CstNodeMap CstNodeMap;
typedef struct _CstNodeMapClass CstNodeMapClass;

typedef struct _CstPropMap CstPropMap;
typedef struct _CstPropMapClass CstPropMapClass;

typedef struct _CstPropPair CstPropPair;
typedef SysValue CstPropValue;

typedef void (*CstNodeMapFunc) (CstNode *v_node, const SysChar *key, const SysChar *bind_var, CstPropValue *value);
typedef void (*CstCssFree) (CstCssValue *value);
typedef void (*CstCssCalcFunc) (CstRenderNode *render_node, SysPointer user_data);

SYS_END_DECLS

#endif

