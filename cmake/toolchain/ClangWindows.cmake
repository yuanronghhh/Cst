if(NOT EXISTS "${CLANG_HOME}/")
  message(FATAL_ERROR "Require CLANG_HOME for ClangWindows ToolChain")
endif()

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR AMD64)
set(CMAKE_GENERATOR_TOOLSET "ClangCL")

set(CMAKE_C_COMPILER "${CLANG_HOME}/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${CLANG_HOME}/bin/clang++.exe")

if(WIN32)
configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/Directory.build.props.in
  ${CMAKE_BINARY_DIR}/Directory.build.props
)
endif()
message(STATUS "USE ClangWindows ToolChain ${CMAKE_C_COMPILER}")
