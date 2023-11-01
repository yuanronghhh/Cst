#ifndef __CST_BUILDER_H__
#define __CST_BUILDER_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define CST_TYPE_BUILDER (cst_builder_get_type())
#define CST_BUILDER(o) ((CstBuilder* )sys_object_cast_check(o, CST_TYPE_BUILDER))
#define CST_BUILDER_CLASS(o) ((CstBuilderClass *)sys_class_cast_check(o, CST_TYPE_BUILDER))
#define CST_BUILDER_GET_CLASS(o) sys_instance_get_class(o, CstBuilderClass)


struct _CstBuilder {
  SysObject parent;

  /* < private > */
  CstContext *c;
};

struct _CstBuilderClass {
  SysObjectClass parent;

  void (*parse) (CstBuilder *self, CstParser *ps);
  void (*build) (CstBuilder *self, SysObject *o);
};

SysType cst_builder_get_type(void);
CstBuilder *cst_builder_new(void);

void cst_builder_parse(CstNodeBuilder *self, CstParser *ps);
void cst_builder_build(CstNodeBuilder *self, SysObject *o);

SYS_END_DECLS

#endif

