#!/usr/bin/env python3
import logging
import os
from os import path
from pathlib import Path

logging.basicConfig(format="%(message)s", level=logging.DEBUG)

def rename_glib(file):
    f = open(file, "r+")
    data = f.read()
    ndata = data.replace("G_", "SYS_")\
            .replace("g_", "sys_")\
            .replace("gboolean", "SysBool")\
            .replace("GReal", "SysReal")\
            .replace("gpointer", "SysPointer")\
            .replace("GError", "SysError")\
            .replace("GThread", "SysThread")\
            .replace("gsize", "SysSize")\
            .replace("GPrivate", "SysPrivate")\
            .replace(" gint", " SysInt")\
            .replace("guint", "SysUInt")\
            .replace("gchar", "SysChar")\
            .replace("gulong", "SysULong")\
            .replace("TRUE", "true")\
            .replace("FALSE", "false")\
            .replace("GMutex", "SysMutex")\
            .replace("GCond", "SysCond")\
            .replace("GSList", "SysSList")\
            .replace("GRWLock", "SysRWLock")\
            .replace("GRecMutex", "SysRecMutex")\
            .replace("compare_and_exchange", "cmpxchg")\
            .replace("g_strerror", "sys_strerr")\
            .replace("sys_set_error", "sys_error_set_N")\
            .replace("sys_strerror", "sys_strerr")\
            .replace("sys_abort", "sys_abort_N")\
            .replace("sys_critical", "sys_abort_N")\
            .replace("GDestroyNotify", "SysDestroyFunc")

    f.seek(0)
    f.truncate()
    f.write(ndata)
    f.close()

if __name__ == '__main__':
    rename_glib("D:/GreyHound/PRIVATE/Git/Cst/Cst/System/Platform/Win32/SysThreadWin32.c")
