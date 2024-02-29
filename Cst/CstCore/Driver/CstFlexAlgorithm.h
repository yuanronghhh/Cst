#ifndef __CST_FLEX_ALGORITHM_H__
#define __CST_FLEX_ALGORITHM_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_FLEX_ALGORITHM (cst_flex_algorithm_get_type())
#define CST_FLEX_ALGORITHM(o) ((CstFlexAlgorithm* )sys_object_cast_check(o, CST_TYPE_FLEX_ALGORITHM))
#define CST_FLEX_ALGORITHM_CLASS(o) ((CstFlexAlgorithmClass *)sys_class_cast_check(o, CST_TYPE_FLEX_ALGORITHM)
#define CST_FLEX_ALGORITHM_GET_CLASS(o) sys_instance_get_class(o, CstFlexAlgorithmClass)

typedef struct _CstFlexAlgorithm CstFlexAlgorithm;
typedef struct _CstFlexAlgorithmClass CstFlexAlgorithmClass;

struct _CstFlexAlgorithmClass {
  SysObjectClass parent;
};

struct _CstFlexAlgorithm {
  SysObject parent;

  /* <private> */
};

SYS_API SysType cst_flex_algorithm_get_type(void);
SYS_API CstFlexAlgorithm *cst_flex_algorithm_new_I(void);

SYS_END_DECLS

#endif
