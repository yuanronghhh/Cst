@echo off

set PRO_NAME=%1
set BUILD_PATH="build"
set SLN_PATH="Cst.sln"

call "D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

msbuild "%BUILD_PATH%/%PRO_NAME%"
