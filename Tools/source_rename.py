#!/usr/bin/env python3
import logging
import os
from os import path
from pathlib import Path

logging.basicConfig(format="%(message)s", level=logging.DEBUG)

def path_suffix(pname):
    return path.splitext(pname)[-1]

def parse_object_define(line):
    if not line.startswith("SYS_DEFINE_TYPE_WITH_PRIVATE"):
        return

    offset = -3
    if not line.endswith(";\n"):
        offset = -2

    dinfo = line[29: offset].split(",")
    for i, item in enumerate(dinfo):
        dinfo[i] = item.strip()

    return dinfo

def update_object_data(f):
    dinfo = []
    lines = f.readlines()
    update = False

    for lnum, line in enumerate(lines):
        if not line:
            break

        if not dinfo:
            dinfo = parse_object_define(line)
            continue
            # if dinfo:
            #     lines[lnum-1] = "SYS_DEFINE_TYPE(%s, %s, %s);\n" % (dinfo[1], dinfo[2], dinfo[0])
            #     break
            # continue

        if dinfo:
            # if line.find("init(SysObject *o)") > -1:
            #     lines[lnum] = line.replace("SysObject *o", dinfo[0] + " *self")
            #     update = True

            # if line.find(dinfo[1] + "_init(" + dinfo[0] + " *self)") > -1:
            #   if lines[lnum + 1].find("priv") == -1:
            #       if lines[lnum + 1].find("}") == -1:
            #           lines[lnum + 1] = "  %s%s" % (dinfo[0] + "Private *priv = self->priv = " + dinfo[1] + "_get_private(self);\n", lines[lnum + 1])
            #       else:
            #           lines[lnum + 1] = "  %s%s" % ("self->priv = " + dinfo[1] + "_get_private(self);\n", lines[lnum + 1])

            #       continue

    if dinfo and update:
        logging.info(dinfo)
        data = "".join(lines)
        f.seek(0)
        f.truncate()
        f.write(data)
        f.close()

def rename_gobject():
    wk = os.getcwd() + "/Cst"

    for dp, dns, fns in os.walk(wk):
        for f in fns:
            if path_suffix(f) not in {".c", ".h" }:
                continue

            fname = path.join(dp, f)
            f = open(fname, "r+", encoding="utf-8")

            update_object_data(f)

            f.close()

if __name__ == '__main__':
    rename_gobject()
