if(NOT EXISTS "${CLANG_HOME}/")
  message(FATAL_ERROR "Require CLANG_HOME for ClangWindows ToolChain")
endif()

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

if(WIN32)
  set(LLVM_PROPS "${CMAKE_BINARY_DIR}/Directory.build.props")
  # if (NOT EXISTS ${LLVM_PROPS})
  #   configure_file(
  #     "${CMAKE_CURRENT_LIST_DIR}/Directory.build.props.in"
  #     "${LLVM_PROPS}"
  #   )
  # endif()
endif()

set(CMAKE_C_COMPILER "${CLANG_HOME}/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${CLANG_HOME}/bin/clang++.exe")

set(CMAKE_GENERATOR_TOOLSET "ClangCL")

message(STATUS "USE ClangWindows ToolChain ${CMAKE_C_COMPILER}")
