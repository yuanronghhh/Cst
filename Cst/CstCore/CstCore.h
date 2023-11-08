#ifndef __CST_CORE_H__
#define __CST_CORE_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#include <CstCore/Driver/Css/CstCssCore.h>

#include <CstCore/Front/CstFrontCore.h>
#include <CstCore/Driver/CstAbsLayer.h>
#include <CstCore/Driver/CstApplication.h>
#include <CstCore/Driver/CstBoxLayer.h>
#include <CstCore/Driver/CstBoxNode.h>
#include <CstCore/Driver/CstCommon.h>
#include <CstCore/Driver/CstComponent.h>
#include <CstCore/Driver/CstContext.h>
#include <CstCore/Driver/CstLayer.h>
#include <CstCore/Driver/CstLayout.h>
#include <CstCore/Driver/CstLayoutFlex.h>
#include <CstCore/Driver/CstLayoutNode.h>
#include <CstCore/Driver/CstModule.h>
#include <CstCore/Driver/CstNode.h>
#include <CstCore/Driver/CstNodeMap.h>
#include <CstCore/Driver/CstValueMap.h>
#include <CstCore/Driver/CstRender.h>
#include <CstCore/Driver/CstRenderContext.h>
#include <CstCore/Driver/CstRenderNode.h>
#include <CstCore/Parser/CstParserCore.h>
#include <CstCore/Driver/CstContext.h>

void cst_core_setup (void);
void cst_core_teardown (void);

SYS_END_DECLS

#endif
