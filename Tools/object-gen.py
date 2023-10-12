#!/usr/bin/env python3

import abc
import argparse
import os
import re
import sys


h_template = """\
#ifndef __${TYPE_NAME}_H__
#define __${TYPE_NAME}_H__

#include <Framework/FRCommon.h>

SYS_BEGIN_DECLS

#define ${TYPE_NAME}_TYPE (${type_name}_get_type())
#define ${TYPE_NAME}(o) ((${TypeName}* )sys_object_cast_check(o, ${TYPE_NAME}_TYPE))
#define ${TYPE_NAME}_CLASS(o) ((${TypeName}Class *)sys_class_cast_check(o, ${TYPE_NAME}_TYPE))
#define ${TYPE_NAME}_GET_CLASS(o) sys_instance_get_class(o, ${TypeName}Class)

typedef struct _${TypeName} ${TypeName};
typedef struct _${TypeName}Class ${TypeName}Class;

struct _${TypeName}Class {
  SysObjectClass parent;
};

${StructType}

SYS_API SysType ${type_name}_get_type(void);
SYS_API ${TypeName} *${type_name}_new_I(void);

SYS_END_DECLS

#endif
"""


c_template = """\
#include <CstCore/Driver/CstCommon.h>

SYS_DEFINE_TYPE(${PARENT_TYPE}, ${TypeName}, ${type_name});

/* object api */
static void ${type_name}_construct(SysObject *o) {
  SYS_OBJECT_CLASS(${type_name}_parent_class)->construct(o);

  ${TypeName}* self = ${TYPE_NAME}(o);
}

${TypeName}* ${type_name}_new(void) {
  return sys_object_new(${TYPE_NAME}_TYPE, NULL);
}

${TypeName} *${type_name}_new_I(void) {
  ${TypeName} *o = ${type_name}_new();

  ${type_name}_construct(SYS_OBJECT(o));

  return o;
}

static void ${type_name}_dispose(SysObject* o) {
  ${TypeName} *self = ${TYPE_NAME}(o);
  ${TypeName}Private* priv = self->priv;

  SYS_OBJECT_CLASS(${type_name}_parent_class)->dispose(o);
}

static void ${type_name}_class_init(${TypeName}Class* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->construct = (SysObjectFunc)${type_name}_construct;
  ocls->dispose = ${type_name}_dispose;
}

void ${type_name}_init(SysObject* o) {
  ${TypeName} *self = ${TYPE_NAME}(o);
  self->priv = ${type_name}_get_private(self);
}

"""

template_struct = """
struct _CstNodeProvider {
  /* <private> */
  CstModule* v_module;
  CstComponent* v_component;
  CstNode* v_parent;
  CstNode* v_node;
};
"""

class TemplateInfo:
    def __init__(self, structStr):
        self.tpl = structStr;
        self.FName = ""
        self.F_NAME = ""
        self.f_name = ""
        self.F_TYPE_NAME = ""
        self.F_TYPE_PARENT_NAME = ""
        self.f_parent_name = ""
        self.FParentName = ""
        self.F_PARENT_NAME = ""
        self.prefix = ""

    def parse_info(self):
        sp = self.tpl.split("\n")
        self.prefix = sp.parse_prefix(sp[0])
        self.FName = sp.parse_struct_name(sp[0])

    def parse_prefix(self, line):
        pass

class TemplateGenerator:

    def __init__(self, structStr, dstDir):
        self.sInfo = new TemplateInfo(structStr)
        self.dstDir = dstDir

    def generate_file(self):
        info = self.sInfo
        h_file = self.dstDir + info.TypeName + ".h"
        c_file = self.dstDir + info.TypeName + ".c"


def main():
    dst = os.getcwd() + "/Cst/Driver/"
    dst = dst.replace("\\", "/")

    gen = TemplateGenerator(template_struct, dst)
    gen.parse_info()
    gen.generate_file()

    for info in infos:
        h_file = dst + info[2] + ".h"
        c_file = dst + info[2] + ".c"

        result = gen_class_code(info)

        fp = open(h_file, "w+")
        fp.write(result[0])
        fp.close()

        fp = open(c_file, "w+")
        fp.write(result[1])
        fp.close()

if __name__ == '__main__':
    main()
