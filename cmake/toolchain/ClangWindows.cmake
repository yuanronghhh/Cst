if(NOT EXISTS "${CLANG_HOME}/")
  message(FATAL_ERROR "Windows requires pre-compiled libs at: '${LIBDIR}'")
endif()

# Directory.build.props file copy to .sln path for visual studio
# <Project>
#   <PropertyGroup>
#     <LLVMInstallDir>D:\GreyHound\PRIVATE\DOWNLOADS\clang+llvm-18.1.4-x86_64-pc-windows-msvc.tar\clang+llvm-18.1.4-x86_64-pc-windows-msvc</LLVMInstallDir>
#     <LLVMToolsVersion>18.1.4</LLVMToolsVersion>
#   </PropertyGroup>
# </Project>

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR AMD64)
set(TRIPLE "x86_64")
set(CMAKE_GENERATOR_TOOLSET "ClangCL")

set(CMAKE_C_COMPILER "${CLANG_HOME}/bin/clang.exe")
set(CMAKE_C_COMPILER_TARGET ${TRIPLE})
message(STATUS "USE COMPILER ${CMAKE_C_COMPILER}")

set(CMAKE_CXX_COMPILER "${CLANG_HOME}/bin/clang++.exe")
set(CMAKE_CXX_COMPILER_TARGET ${TRIPLE})

message(STATUS "USE ClangWindows ToolChain")
