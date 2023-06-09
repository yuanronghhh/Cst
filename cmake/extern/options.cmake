if(WIN32)
  set(CMAKE_FIND_LIBRARY_PREFIXES "")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

  set(SYS_SEARCH_DIRS "$ENV{ProgramW6432}")
  if(NOT ${SYS_SEARCH_DIRS} STREQUAL "")
    STRING(REPLACE "\\" "/" SYS_SEARCH_DIRS ${SYS_SEARCH_DIRS})
  endif()
elseif(UNIX)
  set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so")
endif()
