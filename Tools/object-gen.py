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
struct _CstElement {
  /* <private> */
  CstNode *node;
  CstRenderNode *rnode;
  CstLayerNode *lnode;
};
"""

class TemplateInfo:
    struct_name_re = re.compile(r"struct _(\w+)")
    type_re = re.compile(r"^\s+([a-zA-Z0-9]+\s*\*{0,1}) ")
    prop_re = re.compile(r"\s\+([a-zA-Z0-9_]+)\s*;")

    def __init__(self, structStr, Prefix):
        self.tpl = structStr.strip(" ").split("\n")
        self.F = Prefix.upper()
        self.f = "%s%s" % (Prefix[0].lower(), Prefix[1:])
        self.Name = ""
        self.name = ""
        self.props = []

    def parse_info(self):
        self.Name = self.parse_struct_name(self.tpl[1])
        self.name = "%s%s" % (self.Name[0].lower(), self.Name[1:])
        self.props = self.parse_props(self.tpl[2:-1])

    def parse_prop(self, line):
        prop = [None, None]
        match = TemplateInfo.type_re.findall(line)
        if not match:
            return prop

        prop[0] = match[0]

        match = TemplateInfo.prop_re.findall(line)
        if not match:
            return prop
        prop[1] = match[0]

        return match[0]

    def parse_props(self, lines):
        for line in lines:
            prop = self.parse_prop(line)

    def parse_struct_name(self, line):
        match = TemplateInfo.struct_name_re.findall(line)
        if not match:
            return None

        return match[0][len(self.F):]

    def parse_prefix(self, line):
        match = TemplateInfo.struct_name_re.findall(line)
        if not match:
            return None

        return match[0]

class TemplateGenerator:
    def __init__(self, structStr, prefix, dstDir):
        self.sInfo = TemplateInfo(structStr, prefix)
        self.sInfo.parse_info()

        self.dstDir = dstDir

    def generate_file(self):
        info = self.sInfo
        h_file = self.dstDir + info.TypeName + ".h"
        c_file = self.dstDir + info.TypeName + ".c"


def main():
    dst = os.getcwd() + "/Cst/Driver/"
    dst = dst.replace("\\", "/")

    gen = TemplateGenerator(template_struct, "Cst", dst)
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
