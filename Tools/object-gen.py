#!/usr/bin/env python3

import abc
import argparse
import os
import re
import sys
import logging

from AppJsonEncoder import Serializer
from pathlib import Path

logging.basicConfig(format="%(message)s", level=logging.DEBUG)

h_template = """\
#ifndef __${TYPE_NAME}_H__
#define __${TYPE_NAME}_H__

#include <CstCore/Driver/CstCommon.h>

SYS_BEGIN_DECLS

#define ${FN_TYPE_NAME} (${type_name}_get_type())
#define ${TYPE_NAME}(o) ((${TypeName}* )sys_object_cast_check(o, ${FN_TYPE_NAME}))
#define ${TYPE_NAME}_CLASS(o) ((${TypeName}Class *)sys_class_cast_check(o, ${FN_TYPE_NAME})
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
  return sys_object_new(${FN_TYPE_NAME}, NULL);
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

class Props:
    def __init__(self):
        self.type = None
        self.name = None

class TemplateInfo:
    struct_name_re = re.compile(r"struct _(\w+)")
    type_re = re.compile(r"^\s+([a-zA-Z0-9]+\s*\*{0,1})")
    prop_re = re.compile(r"([a-zA-Z0-9_]+)\s*;")

    def __init__(self, struct_str):
        self.struct_str = struct_str
        self.tpl = struct_str.strip(" ").split("\n")

        self.struct = self.parse_struct(self.tpl[1])
        self.sep_struct = self.seperate_struct(self.struct)
        self.props = self.parse_props(self.tpl[2:-1])

        self.pinfo = self.parse_pinfo(self.tpl[2]);
        if not self.pinfo:
            logging.error("struct not parse parent info: %s", self.tpl[2])
            sys.exit(-1)
        else:
            self.p_sep_struct = self.seperate_struct(self.pinfo.type)

    def parse_prop(self, line):
        prop = Props()
        if line.startswith("#"):
            return None

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
        if not props:
            return None
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

    def is_satify(self, b, i, c, nc):
        if not nc:
            return True

        if i <= b:
            return False

        return True

    def seperate_struct(self, struct_name):
        b = 0
        data = []
        slen = len(struct_name)

        for i in range(0, slen):
            c = struct_name[i]

            if i < slen-1:
                if not c.isupper():
                    continue

            nc = None
            if i+1 < slen:
                nc = struct_name[i+1]

            if self.is_satify(b, i, c, nc):
                if nc:
                    data.append(struct_name[b:i])
                else:
                    data.append(struct_name[b:i+1])

                b = i

        return data

    def parse_Prefix(self, struct_name):
        data = self.seperate_struct(struct_name)
        if not data:
            return None

        return data[0]

    def get_TypeName(self):
        return "".join(self.sep_struct)

    def get_typename(self):
        return "".join(self.sep_struct).lower()

    def get_type_name(self):
        return "_".join(self.sep_struct).lower()

    def get_struct_str(self):
        return self.struct_str

    def get_TYPE_NAME(self):
        return "_".join(self.sep_struct).upper()

    def get_PARENT_TYPE(self):
        if not self.pinfo:
            return ""

        return "_".join(self.p_sep_struct).upper()

    def get_ParentType(self):
        if not self.pinfo:
            return ""

        return "".join(self.p_sep_struct)

    def get_FN_TYPE_NAME(self):
        if not self.pinfo:
            return ""

        return ("%s_TYPE_%s" % (self.sep_struct[0], "_".join(self.sep_struct[1:]))).upper()

    def get_TYPE_PARENT(self):
        if not self.pinfo:
            return ""

        return ("%s_TYPE_%s" % (self.p_sep_struct[0], "_".join(self.p_sep_struct[1:]))).upper()

class TemplateGenerator:
    def __init__(self, structStr, dstDir):
        self.sInfo = TemplateInfo(structStr)
        self.dstDir = dstDir

    def gen_with_tpl(self, tpl, info):
        r = tpl.replace("${TYPE_NAME}", info.get_TYPE_NAME())\
                .replace("${FN_TYPE_NAME}", info.get_FN_TYPE_NAME())\
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
        h_file = self.dstDir + "/" + info.get_TypeName() + ".h"
        c_file = self.dstDir + "/" + info.get_TypeName() + ".c"

        result = self.gen_with_tpl(h_template, info)
        fp = open(h_file, "w+")
        fp.write(result)
        fp.close()

        result = self.gen_with_tpl(c_template, info)
        fp = open(c_file, "w+")
        fp.write(result)
        fp.close()


template_struct = """
struct _CstFlexAlgorithm {
  SysObject parent;

  /* <private> */
};
"""

def main():
    dst = Path("D:/GreyHound/PRIVATE/Git/Cst/Cst/CstCore/Driver").absolute().as_posix()

    gen = TemplateGenerator(template_struct, dst)
    gen.generate_file()

if __name__ == '__main__':
    main()
