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

def rename_surffix(fname, nname):
    logging.info("%s\t%s" % (fname, nname))
    os.rename(fname, nname)

def rename_data(fname):
    p1 = fname.name.replace("Fr", "FR").replace(".h", "")
    p2 = fname.name.replace(".h", "")
    # print(".replace(\"%s\",\"%s\") \\" % (p1, p2))
    # return

    f = open(fname, "r+", encoding="utf-8")
    data = f.read()
    data = data\
            .replace("FRPServer", "FrpServer") \
            .replace("FRRegion", "FrRegion") \
            .replace("FRSurface", "FrSurface") \
            .replace("FRContext", "FrContext") \
            .replace("FRSInt", "FrSInt") \
            .replace("FRColor", "FrColor") \
            .replace("FRRect", "FrRect") \
            .replace("FRGetBoundFunc", "FrGetBoundFunc") \
            .replace("FRCommon","FrCommon") \
            .replace("FRTypes","FrTypes") \
            .replace("FRCore","FrCore") \
            .replace("FRNode","FrNode") \
            .replace("FRApplication","FrApplication") \
            .replace("FRSource","FrSource") \
            .replace("FRMain","FrMain") \
            .replace("FRWorker","FrWorker") \
            .replace("FREnv","FrEnv") \
            .replace("FRPair","FrPair") \
            .replace("FRFunc","FrFunc") \
            .replace("FRPQueue","FrPQueue") \
            .replace("FRDraw","FrDraw") \
            .replace("FRGraph","FrGraph") \
            .replace("FRCanvasPrivate","FrCanvasPrivate") \
            .replace("FRCanvas","FrCanvas") \
            .replace("FREventCore","FrEventCore") \
            .replace("FREvents","FrEvents") \
            .replace("FRAWatchBuilder","FrAWatchBuilder") \
            .replace("FRAction","FrAction") \
            .replace("FRAWatchCursorMove","FrAWatchCursorMove") \
            .replace("FRAMouseKey","FrAMouseKey") \
            .replace("FRAWatchAny","FrAWatchAny") \
            .replace("FRAWatchMousePress","FrAWatchMousePress") \
            .replace("FRADoubleClick","FrADoubleClick") \
            .replace("FRAWatchKey","FrAWatchKey") \
            .replace("FRAWatch","FrAWatch") \
            .replace("FRAWatchKeyPress","FrAWatchKeyPress") \
            .replace("FRAWatchRefresh","FrAWatchRefresh") \
            .replace("FRACursorMove","FrACursorMove") \
            .replace("FRAKey","FrAKey") \
            .replace("FRAWatchMouseRelease","FrAWatchMouseRelease") \
            .replace("FREventMapping","FrEventMapping") \
            .replace("FREvent","FrEvent") \
            .replace("FREventScroll","FrEventScroll") \
            .replace("FREventNodeKey","FrEventNodeKey") \
            .replace("FREventRefresh","FrEventRefresh") \
            .replace("FREventAny","FrEventAny") \
            .replace("FREventCursorMove","FrEventCursorMove") \
            .replace("FREventMouseKey","FrEventMouseKey") \
            .replace("FREventKey","FrEventKey") \
            .replace("FREventPrivate","FrEventPrivate") \
            .replace("FREventPressed","FrEventPressed") \
            .replace("FRExpr","FrExpr") \
            .replace("FRFont","FrFont") \
            .replace("FRVkvg","FrVkvg") \
            .replace("FRMono","FrMono") \
            .replace("FRGlfw","FrGlfw") \
            .replace("FRCairo","FrCairo") \
            .replace("FRGlib","FrGlib") \
            .replace("FRWindow","FrWindow") \
            .replace("FRDisplay","FrDisplay")

    f.seek(0)
    f.truncate()
    f.write(data)
    f.close()

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
            pass
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
    wk = "/home/greyhound/Git/CstDemo/Cst/CstDemo"

    for dp, dns, fns in os.walk(wk):
        for f in fns:
            if path_suffix(f) not in {".c", ".h" }:
                continue

            fname = Path(path.join(dp, f))
            # if not fname.name.startswith("App"):
            #     continue

            nname = "%s/%s" % (fname.parent.as_posix(), fname.name.replace("FR", "Fr"))
            rename_data(fname)

if __name__ == '__main__':
    rename_gobject()
