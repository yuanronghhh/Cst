cmake_minimum_required(VERSION 3.5)

if(NOT EXECUTABLE_OUTPUT_PATH)
    set(FIRST_RUN TRUE)
else()
    set(FIRST_RUN FALSE)
endif()

include(cmake/init.cmake)
include(cmake/config/build_option.cmake)

if(USE_MONO)
  set(CMAKE_Csharp_COMPILER "C:/Program Files/Mono/bin/mcs.bat")
  project(Cst LANGUAGES C CSharp)
else()
  project(Cst LANGUAGES C)
endif()

add_subdirectory(Cst)

configure_file(${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt.in
               ${CMAKE_CURRENT_LIST_DIR}/compile_flags.txt
)
