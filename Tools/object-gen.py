#!/usr/bin/env python3

import abc
import argparse
import os
import re
import sys

from AppJsonEncoder import Serializer


h_template = """\
#ifndef __${TYPE_NAME}_H__
#define __${TYPE_NAME}_H__

#include <CstCore/Driver/CstCommon.h>

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
${struct_str}
SYS_API SysType ${type_name}_get_type(void);
SYS_API ${TypeName} *${type_name}_new_I(void);

SYS_END_DECLS

#endif
"""


c_template = """\
#include <CstCore/Driver/${TypeName}.h>

SYS_DEFINE_TYPE(${TypeName}, ${type_name}, ${TYPE_PARENT});


/* object api */
static void ${type_name}_construct(${TypeName} *self) {

}

${TypeName}* ${type_name}_new(void) {
  return sys_object_new(${TYPE_NAME}_TYPE, NULL);
}

${TypeName} *${type_name}_new_I(void) {
  ${TypeName} *o = ${type_name}_new();

  ${type_name}_construct(o);

  return o;
}

static void ${type_name}_dispose(SysObject* o) {
  ${TypeName} *self = ${TYPE_NAME}(o);

  SYS_OBJECT_CLASS(${type_name}_parent_class)->dispose(o);
}

static void ${type_name}_class_init(${TypeName}Class* cls) {
  SysObjectClass *ocls = SYS_OBJECT_CLASS(cls);

  ocls->dispose = ${type_name}_dispose;
}

void ${type_name}_init(${TypeName}* self) {
}

"""

template_struct = """
struct _CstElement {
  SysObject unowned;

  /* <private> */
  CstNode *node;
  CstRenderNode *rnode;
  CstLayerNode *lnode;
};
"""

class Props:
    def __init__(self):
        self.type = None
        self.name = None

class TemplateInfo:
    struct_name_re = re.compile(r"struct _(\w+)")
    type_re = re.compile(r"^\s+([a-zA-Z0-9]+\s*\*{0,1})")
    prop_re = re.compile(r"([a-zA-Z0-9_]+)\s*;")

    def __init__(self, struct_str, Prefix):
        self.struct_str = struct_str
        self.tpl = struct_str.strip(" ").split("\n")
        self.struct = self.parse_struct(self.tpl[1])
        Prefix = Prefix if Prefix else self.parse_Prefix(self.struct)

        self.Fn = Prefix
        self.FN = self.Fn.upper()
        self.fn = "%s%s" % (self.Fn[0].lower(), self.Fn[1:])

        self.Name = self.parse_struct_name(self.Fn, self.struct)
        self.name = "%s%s" % (self.Name[0].lower(), self.Name[1:])
        self.NAME = self.Name.upper()
        self.props = self.parse_props(self.tpl[2:-1])

        self.pinfo = self.parse_pinfo(self.tpl[2]);
        self.p_Fn = self.parse_Prefix(self.pinfo.type)
        self.p_FN = self.p_Fn.upper()
        self.p_fn = "%s%s" % (self.p_Fn[0].lower(), self.p_Fn[1:])

        self.p_Name = self.parse_struct_name(self.p_Fn, self.pinfo.type)
        self.p_name = "%s%s" % (self.p_Name[0].lower(), self.p_Name[1:])
        self.p_NAME = self.p_Name.upper()

    def parse_prop(self, line):
        prop = Props()
        match = TemplateInfo.type_re.findall(line)
        if not match:
            return None

        prop.type = match[0].strip()

        match = TemplateInfo.prop_re.findall(line)
        if not match:
            return None
        prop.name = match[0]

        return prop

    def parse_pinfo(self, first_str):
        props = self.parse_props([first_str])
        return props[0]

    def parse_props(self, lines):
        props = []
        for line in lines:
            prop = self.parse_prop(line)
            if not prop:
                continue

            props.append(prop)
        return props

    def parse_struct(self, line):
        match = TemplateInfo.struct_name_re.findall(line)
        if not match:
            return None

        return match[0]


    def parse_struct_name(self, prefix, struct):
        return struct.replace(prefix, "")

    def parse_Prefix(self, struct_name):
        p = 0
        first = True

        for i in struct_name:
            p += 1
            if i > 'A' and i < 'Z':
                if first:
                    first = False
                    continue
                else:
                    break

        return struct_name[0: p-1]

    def get_TypeName(self):
        return "%s%s" % (self.Fn, self.Name)

    def get_typename(self):
        return "%s%s" % (self.Fn, self.name)

    def get_type_name(self):
        return "%s_%s" % (self.fn, self.name)

    def get_struct_str(self):
        return self.struct_str

    def get_TYPE_NAME(self):
        return "%s_%s" % (self.FN, self.NAME)

    def get_PARENT_TYPE(self):
        return "%s_%s" % (self.p_FN, self.p_NAME)

    def get_ParentType(self):
        return "%s%s" % (self.p_Fn, self.p_Name)

    def get_TYPE_PARENT(self):
        return "%s_TYPE_%s" % (self.p_FN, self.p_NAME)

class TemplateGenerator:
    def __init__(self, structStr, Prefix, dstDir):
        self.sInfo = TemplateInfo(structStr, Prefix)
        self.dstDir = dstDir

    def gen_with_tpl(self, tpl, info):
        r = tpl.replace("${TYPE_NAME}", info.get_TYPE_NAME())\
                .replace("${PARENT_TYPE}", info.get_PARENT_TYPE())\
                .replace("${TYPE_PARENT}", info.get_TYPE_PARENT())\
                .replace("${ParentType}", info.get_ParentType())\
                .replace("${typename}", info.get_typename())\
                .replace("${TypeName}", info.get_TypeName())\
                .replace("${type_name}", info.get_type_name())\
                .replace("${struct_str}", info.get_struct_str())

        return r

    def generate_file(self):
        info = self.sInfo
        h_file = self.dstDir + info.get_TypeName() + ".h"
        c_file = self.dstDir + info.get_TypeName() + ".c"

        result = self.gen_with_tpl(h_template, info)
        fp = open(h_file, "w+")
        fp.write(result)
        fp.close()

        result = self.gen_with_tpl(c_template, info)
        fp = open(c_file, "w+")
        fp.write(result)
        fp.close()

def main():
    dst = os.getcwd() + "/Cst/CstCore/Driver/"
    dst = dst.replace("\\", "/")

    gen = TemplateGenerator(template_struct, None, dst)
    gen.generate_file()

if __name__ == '__main__':
    main()
