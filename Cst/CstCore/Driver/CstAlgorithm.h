#ifndef __CST_ALGORITHM_H__
#define __CST_ALGORITHM_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_ALGORITHM (cst_algorithm_get_type())
#define CST_ALGORITHM(o) ((CstAlgorithm* )sys_object_cast_check(o, CST_TYPE_ALGORITHM))
#define CST_ALGORITHM_CLASS(o) ((CstAlgorithmClass *)sys_class_cast_check(o, CST_TYPE_ALGORITHM)
#define CST_ALGORITHM_GET_CLASS(o) sys_instance_get_class(o, CstAlgorithmClass)

struct _CstAlgorithmClass {
  SysObjectClass parent;

  void (*layout) (CstAlgorithm *self, CstRenderNode* rnode, CstLayout* layout);
};

struct _CstAlgorithm {
  SysObject parent;

  /* <private> */
};

SYS_API SysType cst_algorithm_get_type(void);
SYS_API CstAlgorithm* cst_algorithm_new_I(void);
SYS_API void cst_algorithm_layout(CstAlgorithm *self, CstRenderNode * rnode, CstLayout * layout);

SYS_END_DECLS

#endif
