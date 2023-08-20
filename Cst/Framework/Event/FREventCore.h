#ifndef __FR_EVENT_CORE_H__
#define __FR_EVENT_CORE_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <Framework/Event/Action/FRAWatchAny.h>
#include <Framework/Event/Action/FRAWatchRefresh.h>
#include <Framework/Event/Action/FRAWatchCursorMove.h>
#include <Framework/Event/Action/FRAWatchMousePress.h>
#include <Framework/Event/Action/FRAWatchKey.h>
#include <Framework/Event/Action/FRAWatchKeyPress.h>
#include <Framework/Event/Action/FRAWatch.h>

#include <Framework/Event/Action/FRACursorMove.h>
#include <Framework/Event/Action/FRAMouseKey.h>
#include <Framework/Event/Action/FRAKey.h>
#include <Framework/Event/Action/FRAction.h>

#include <Framework/Event/Base/FREventCursorMove.h>
#include <Framework/Event/Base/FREventMouseKey.h>
#include <Framework/Event/Base/FREventKey.h>
#include <Framework/Event/Base/FREventRefresh.h>
#include <Framework/Event/Base/FREvent.h>
#include <Framework/Event/Base/FREventAny.h>

#include <Framework/Event/FREvents.h>

SYS_END_DECLS

#endif
