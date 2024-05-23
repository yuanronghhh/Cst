cmake_minimum_required(VERSION 3.5)

if(NOT EXECUTABLE_OUTPUT_PATH)
    set(FIRST_RUN TRUE)
else()
    set(FIRST_RUN FALSE)
endif()

SET(INIT_HOME "${CMAKE_CURRENT_LIST_DIR}/cmake")
SET(CMAKE_TOOLCHAIN_FILE ${INIT_HOME}/toolchain/ClangWindows.cmake)
include(cmake/init.cmake)
include(cmake/config/build_option.cmake)

if(USE_MONO)
  set(CMAKE_Csharp_COMPILER "C:/Program Files/Mono/bin/mcs.bat")
  project(Cst LANGUAGES C CSharp)
else()
  project(Cst LANGUAGES C)
endif()

# add_subdirectory("${CMAKE_SOURCE_DIR}/../CstDemo/Cst/System" ${CMAKE_BINARY_DIR}/CstDemo/Cst/System)
add_subdirectory(Framework)
add_subdirectory(FrameworkTest)
add_subdirectory(CstCore)
add_subdirectory(CstCli)
add_subdirectory(CstCoreTest)
if(USE_MONO)
  add_subdirectory(CstMono)
endif()
add_subdirectory(Mini)

configure_file(${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt.in
               ${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt
)

