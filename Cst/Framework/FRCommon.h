#ifndef __FR_COMMON_H__
#define __FR_COMMON_H__

#include <System/SysCore.h>

SYS_BEGIN_DECLS

#include <Framework/ThirdParty/FRFont.h>
#include <Framework/ThirdParty/FRGlib.h>
#include <Framework/ThirdParty/FRCairo.h>
#include <Framework/ThirdParty/FRGlfw.h>
#include <Framework/Event/Base/FREventMapping.h>
#include <Framework/FRTypes.h>

SYS_API SysInt fr_get_type_by_name(const SysChar *names[], SysInt len, const SysChar *name);

SYS_END_DECLS

#endif
