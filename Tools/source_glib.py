#!/usr/bin/env python3
import logging
import os
from os import path
from pathlib import Path

logging.basicConfig(format="%(message)s", level=logging.DEBUG)

def rename_glib(file):
    f = open(file, "r+", encoding="utf-8")
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
            .replace("gint", "SysInt")\
            .replace("gushort", "SysUShort")\
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
            .replace("sys_warning", "sys_warning_N")\
            .replace("sys_critical", "sys_abort_N")\
            .replace("GPtrArray", "SysPtrArray")\
            .replace("GDestroyNotify", "SysDestroyFunc")\
            .replace("GHashTable", "SysHashTable")\
            .replace("GIdleSource", "SysIdleSource")\
            .replace("GTimeoutSource", "SysTimeoutSource")\
            .replace("GPoll", "SysPoll")\
            .replace("GSource", "SysSource")\
            .replace("GChildWatchSource", "SysChildWatchSource")\
            .replace("GMain", "SysMain")\
            .replace("GWin32PollThreadData", "SysWin32PollThreadData")\
            .replace("GList", "SysList")\
            .replace("GPollFD", "SysPollFD")\
            .replace("GWakeup", "SysWakeup")\
            .replace("GLIB_AVAILABLE_STATIC_INLINE_IN_2_70\n", "")\
            .replace("GLIB_AVAILABLE_IN_2_74\n", "")\
            .replace("GLIB_AVAILABLE_IN_ALL\n", "")\
            .replace("GLIB_.*", "")\
            .replace("GLIB_VAR", "")\
            .replace("GIOCondition", "SysIOCondition")\
            .replace("GUnixSignalWatchSource", "SysUnixSignalWatchSource")

    f.seek(0)
    f.truncate()
    f.write(ndata)
    f.close()

if __name__ == '__main__':
    rename_glib("D:/GreyHound/PRIVATE/Git/Cst/Cst/Framework/DataType/FRPoll.c")
