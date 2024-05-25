set(LLVM_HOME "$ENV{LLVM_BIN}/..")
if(NOT EXISTS "${LLVM_HOME}")
  message(FATAL_ERROR "Require LLVM_BIN for ClangWindows ToolChain")
endif()
STRING(REPLACE "\\" "/" LLVM_HOME ${LLVM_HOME})
set(LLVM_VERSION "18.1.4")

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

if(WIN32)
  set(LLVM_PROPS "${CMAKE_BINARY_DIR}/Directory.build.props")
  if (NOT EXISTS ${LLVM_PROPS})
    configure_file(
      "${CMAKE_CURRENT_LIST_DIR}/Directory.build.props.in"
      "${LLVM_PROPS}"
    )
  endif()
endif()

set(CMAKE_C_COMPILER "${LLVM_HOME}/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${LLVM_HOME}/bin/clang++.exe")

message(STATUS "USE ClangWindows ToolChain ${CMAKE_C_COMPILER}")
