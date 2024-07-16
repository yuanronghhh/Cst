cmake_minimum_required(VERSION 3.5)

if(NOT EXECUTABLE_OUTPUT_PATH)
    set(FIRST_RUN TRUE)
else()
    set(FIRST_RUN FALSE)
endif()

SET(INIT_HOME "${CMAKE_CURRENT_LIST_DIR}/../CstDemo/CstCmake/cmake")
include(cmake/build_option.cmake)
include(${INIT_HOME}/init.cmake)

project(Cst)
enable_language(C)

# add_subdirectory("${CMAKE_SOURCE_DIR}/../CstDemo/System" ${CMAKE_BINARY_DIR}/CstDemo/System)
add_subdirectory(Framework)
add_subdirectory(FrameworkTest)
add_subdirectory(CstCore)
add_subdirectory(CstCli)
add_subdirectory(CstCoreTest)
if(USE_MONO)
  add_subdirectory(CstMono)
endif()
# add_subdirectory(Mini)

configure_file(${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt.in
               ${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt
)

