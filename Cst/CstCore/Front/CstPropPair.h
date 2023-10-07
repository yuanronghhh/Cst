#ifndef __CST_PROP_PAIR__
#define __CST_PROP_PAIR__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

void cst_prop_pair_free(CstPropPair * pair);
CstPropPair* cst_prop_pair_new(SysChar * key, SysValue * value);

SysValue* cst_prop_pair_value(CstPropPair * pair);
SysChar* cst_prop_pair_key(CstPropPair * pair);

SYS_END_DECLS

#endif

