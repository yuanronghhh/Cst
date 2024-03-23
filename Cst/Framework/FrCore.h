#ifndef __FR_CORE_H__
#define __FR_CORE_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <Framework/Graph/FrGraph.h>
#include <Framework/Graph/FrCanvasPrivate.h>
#include <Framework/Graph/FrCanvas.h>
#include <Framework/Graph/FrDraw.h>

#include <Framework/DataType/FrMain.h>
#include <Framework/DataType/FrSource.h>
#include <Framework/DataType/FrApplication.h>
#include <Framework/DataType/FrWorker.h>
#include <Framework/DataType/FrPQueue.h>
#include <Framework/DataType/FrPair.h>
#include <Framework/DataType/FrNode.h>
#include <Framework/DataType/FrEnv.h>
#include <Framework/DataType/FrFunc.h>

#include <Framework/Device/FrDisplay.h>
#include <Framework/Device/FrWindow.h>

#include <Framework/FrTypes.h>
#include <Framework/FrCommon.h>
#include <Framework/Event/FrEventCore.h>

#include <Framework/ThirdParty/FrGlib.h>
#include <Framework/ThirdParty/FrGlfw.h>
#include <Framework/ThirdParty/FrCairo.h>
#include <Framework/ThirdParty/FrFont.h>

void fr_core_setup(void);
void fr_core_teardown(void);

SYS_END_DECLS

#endif
