cmake_minimum_required(VERSION 3.5)

if(NOT EXECUTABLE_OUTPUT_PATH)
    set(FIRST_RUN TRUE)
else()
    set(FIRST_RUN FALSE)
endif()

include(cmake/init.cmake)

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo")
log("${CMAKE_BUILD_TYPE}")
if(USE_MONO)
  set(CMAKE_Csharp_COMPILER "C:/Program Files/Mono/bin/mcs.bat")
  project(Cst LANGUAGES C CSharp)
else()
  project(Cst LANGUAGES C)
endif()

add_subdirectory(Cst)
