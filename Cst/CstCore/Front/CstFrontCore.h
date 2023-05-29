#ifndef __CST_FRONT_CORE_H__
#define __CST_FRONT_CORE_H__

#include <CstCore/Front/CstFrontCommon.h>

SYS_BEGIN_DECLS

#if !CST_USE_MONO
#include <CstCore/Front/C/CstCWidget.h>
#include <CstCore/Front/C/CstCLBody.h>
#include <CstCore/Front/C/CstCText.h>
#include <CstCore/Front/C/CstCLGrid.h>
#include <CstCore/Front/C/CstCLBox.h>
#include <CstCore/Front/C/CstCLDiv.h>
#include <CstCore/Front/C/CstCComponent.h>
#else
#include <CstCore/Front/Mono/CstMWidget.h>
#include <CstCore/Front/Mono/CstMLBody.h>
#include <CstCore/Front/Mono/CstMText.h>
#include <CstCore/Front/Mono/CstMLGrid.h>
#include <CstCore/Front/Mono/CstMLBox.h>
#include <CstCore/Front/Mono/CstMLDiv.h>
#include <CstCore/Front/Mono/CstMComponent.h>
#endif

#include <CstCore/Front/Common/CstWidget.h>
#include <CstCore/Front/Common/CstLBody.h>
#include <CstCore/Front/Common/CstText.h>
#include <CstCore/Front/Common/CstLGrid.h>
#include <CstCore/Front/Common/CstLBox.h>
#include <CstCore/Front/Common/CstLDiv.h>
#include <CstCore/Front/Common/CstComponent.h>

SYS_END_DECLS

#endif

