#ifndef __FR_COMMON_H__
#define __FR_COMMON_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <Framework/ThirdParty/FrFont.h>
#include <Framework/ThirdParty/FrGlib.h>
#include <Framework/ThirdParty/FrLibav.h>
#include <Framework/ThirdParty/FrCairo.h>
#include <Framework/ThirdParty/FrGlfw.h>
#include <Framework/ThirdParty/FrSDL2.h>
#include <Framework/ThirdParty/FrFont.h>
#include <Framework/Graph/FrGraph.h>
#include <Framework/Event/Base/FrEventMapping.h>
#include <Framework/FrTypes.h>

SYS_API SysInt fr_get_type_by_name(const SysChar *names[], SysInt len, const SysChar *name);
SYS_API const SysChar* fr_get_name_by_type(const SysChar *names[], SysInt len, SysInt type);

SYS_END_DECLS

#endif
