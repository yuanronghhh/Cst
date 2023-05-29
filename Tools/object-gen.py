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
typedef struct _${TypeName}Private ${TypeName}Private;

struct _${TypeName}Class {
  SysObjectClass parent;
};

struct _${TypeName} {
  SysObject parent;

  ${TypeName}Private *priv;
};

SYS_API SysType ${type_name}_get_type(void);
SYS_API ${TypeName} *${type_name}_new_I(void);

SYS_END_DECLS

#endif
"""


c_template = """\
#include <Framework/Event/Action/${TypeName}.h>

struct _${TypeName}Private {
  SysInt reserved;
};

SYS_DEFINE_TYPE(FR_EVENT_TYPE, ${TypeName}, ${type_name});

/* object api */
static void ${type_name}_construct(SysObject *o) {
  SYS_OBJECT_CLASS(${type_name}_parent_class)->construct(o);

  ${TypeName}* self = ${TYPE_NAME}(o);
  ${TypeName}Private* priv = self->priv;


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

def gen_class_code(info):
    h_result = ""
    c_result = ""

    h_result = h_template\
            .replace("${TYPE_NAME}", info[0])\
            .replace("${type_name}", info[1])\
            .replace("${TypeName}", info[2])

    c_result = c_template\
            .replace("${TYPE_NAME}", info[0])\
            .replace("${type_name}", info[1])\
            .replace("${TypeName}", info[2])

    return [h_result, c_result]

def main():
    dst = os.getcwd() + "/Cst/Framework/Event/Action/"
    dst = dst.replace("\\", "/")

    infos = [
            ["FR_AAREA",    "fr_aarea",       "FRAArea"],
            ]

    for info in infos:
        h_file = dst + info[2] + ".h"
        c_file = dst + info[2] + ".c"

        # print("#include <Framework/Event/Action/%s%s>" % (info[2], ".h"))

        result = gen_class_code(info)

        fp = open(h_file, "w+")
        fp.write(result[0])
        fp.close()

        fp = open(c_file, "w+")
        fp.write(result[1])
        fp.close()

if __name__ == '__main__':
    main()
