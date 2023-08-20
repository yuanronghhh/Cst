#ifndef __CST_CTYPE__
#define __CST_CTYPE__

#include <CstCore/CstConfig.h>
#include <Framework/FRCore.h>

SYS_BEGIN_DECLS

#define _CST_TYPE_COMPONENT (cst_component_get_type())
#define _CST_COMPONENT(o) ((CstComponent* )sys_object_cast_check(o, _CST_TYPE_COMPONENT))
#define _CST_COMPONENT_CLASS(o) ((CstComponentClass *)sys_class_cast_check(o, _CST_TYPE_COMPONENT))
#define _CST_COMPONENT_GET_CLASS(o) sys_instance_get_class(o, CstComponentClass)

#define _CST_TYPE_NODE (cst_node_get_type())
#define _CST_NODE(o) ((CstNode* )sys_object_cast_check(o, _CST_TYPE_NODE))
#define _CST_NODE_CLASS(o) ((CstNodeClass *)sys_class_cast_check(o, _CST_TYPE_NODE))
#define _CST_NODE_GET_CLASS(o) sys_instance_get_class(o, CstNodeClass)

#define _CST_TYPE_COM_NODE (cst_com_node_get_type())
#define _CST_COM_NODE(o) ((CstComNode* )sys_object_cast_check(o, _CST_TYPE_COM_NODE))
#define _CST_COM_NODE_CLASS(o) ((CstComNodeClass *)sys_class_cast_check(o, _CST_TYPE_COM_NODE))
#define _CST_COM_NODE_GET_CLASS(o) sys_instance_get_class(o, CstComNodeClass)

#define _CST_TYPE_TEXT (cst_text_get_type())
#define _CST_TEXT(o) ((CstText* )sys_object_cast_check(o, _CST_TYPE_TEXT))
#define _CST_TEXT_CLASS(o) ((CstTextClass *)sys_class_cast_check(o, _CST_TYPE_TEXT))
#define _CST_TEXT_GET_CLASS(o) sys_instance_get_class(o, CstTextClass)

#define _CST_TYPE_WIDGET (cst_widget_get_type())
#define _CST_WIDGET(o) ((CstWidget* )sys_object_cast_check(o, _CST_TYPE_WIDGET))
#define _CST_WIDGET_CLASS(o) ((CstWidgetClass *)sys_class_cast_check(o, _CST_TYPE_WIDGET))
#define _CST_WIDGET_GET_CLASS(o) sys_instance_get_class(o, CstWidgetClass)

#define _CST_TYPE_LBODY (cst_lbody_get_type())
#define _CST_LBODY_CLASS(o) ((CstLBodyClass *)sys_class_cast_check(o, _CST_TYPE_LBODY))
#define _CST_LBODY_GET_CLASS(o) sys_instance_get_class(o, CstLBodyClass)
#define _CST_LBODY(o) ((CstLBody* )sys_object_cast_check(o, _CST_TYPE_LBODY))

#define _CST_TYPE_LGRID (cst_lgrid_get_type())
#define _CST_LGRID_CLASS(o) ((CstLGridClass *)sys_class_cast_check(o, _CST_TYPE_LGRID))
#define _CST_LGRID_GET_CLASS(o) sys_instance_get_class(o, CstLGridClass)
#define _CST_LGRID(o) ((CstLGrid* )sys_object_cast_check(o, _CST_TYPE_LGRID))

#define _CST_TYPE_LBOX (cst_lbox_get_type())
#define _CST_LBOX_CLASS(o) ((CstLBoxClass *)sys_class_cast_check(o, _CST_TYPE_LBOX))
#define _CST_LBOX_GET_CLASS(o) sys_instance_get_class(o, CstLBoxClass)
#define _CST_LBOX(o) ((CstLBox* )sys_object_cast_check(o, _CST_TYPE_LBOX))

#define _CST_TYPE_LDIV (cst_ldiv_get_type())
#define _CST_LDIV_CLASS(o) ((CstLDivClass *)sys_class_cast_check(o, _CST_TYPE_LDIV))
#define _CST_LDIV_GET_CLASS(o) sys_instance_get_class(o, CstLDivClass)
#define _CST_LDIV(o) ((CstLDiv* )sys_object_cast_check(o, _CST_TYPE_LDIV))

typedef struct _CstNode CstNode;
typedef struct _CstNodeClass CstNodeClass;
typedef struct _CstNodePrivate CstNodePrivate;

typedef struct _CstLayout CstLayout;
typedef struct _CstLayoutClass CstLayoutClass;
typedef struct _CstLayoutPrivate CstLayoutPrivate;

typedef struct _CstLayoutNode CstLayoutNode;
typedef struct _CstLayoutNodeClass CstLayoutNodeClass;
typedef struct _CstLayoutNodePrivate CstLayoutNodePrivate;

typedef struct _CstWidget CstWidget;
typedef struct _CstWidgetClass CstWidgetClass;
typedef struct _CstWidgetPrivate CstWidgetPrivate;

typedef struct _CstComNode CstComNode;
typedef struct _CstComNodeClass CstComNodeClass;
typedef struct _CstComNodePrivate CstComNodePrivate;

typedef struct _CstComponent CstComponent;
typedef struct _CstComponentClass CstComponentClass;
typedef struct _CstComponentPrivate CstComponentPrivate;

typedef struct _CstText CstText;
typedef struct _CstTextClass CstTextClass;
typedef struct _CstTextPrivate CstTextPrivate;

typedef struct _CstLBox CstLBox;
typedef struct _CstLBoxClass CstLBoxClass;
typedef struct _CstLBoxPrivate CstLBoxPrivate;

typedef struct _CstLDiv CstLDiv;
typedef struct _CstLDivClass CstLDivClass;
typedef struct _CstLDivPrivate CstLDivPrivate;

typedef struct _CstLFlow CstLFlow;
typedef struct _CstLFlowClass CstLFlowClass;
typedef struct _CstLFlowPrivate CstLFlowPrivate;

typedef struct _CstLBody CstLBody;
typedef struct _CstLBodyClass CstLBodyClass;
typedef struct _CstLBodyPrivate CstLBodyPrivate;

typedef struct _CstLFlex CstLFlex;
typedef struct _CstLFlexClass CstLFlexClass;
typedef struct _CstLFlexPrivate CstLFlexPrivate;

typedef struct _CstLGrid CstLGrid;
typedef struct _CstLGridClass CstLGridClass;
typedef struct _CstLGridPrivate CstLGridPrivate;

SYS_END_DECLS

#endif
