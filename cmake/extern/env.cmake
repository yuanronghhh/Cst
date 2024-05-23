if(WIN32)
  set(CMAKE_FIND_LIBRARY_PREFIXES "")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
  set(OS_ARCHITECH "Win64")
  set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.39.33519/bin/Hostx64/x64/cl.exe")

  set(CMAKE_LIBRARY_SUFFIX "lib")
  set(CMAKE_DLL_SUFFIXE "dll")

  set(SYS_SEARCH_DIRS "$ENV{ProgramW6432}")
  if(NOT ${SYS_SEARCH_DIRS} STREQUAL "")
    STRING(REPLACE "\\" "/" SYS_SEARCH_DIRS ${SYS_SEARCH_DIRS})
  endif()
elseif(UNIX)
  set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so")

  set(CMAKE_LIBRARY_PREFIX "lib")
  set(CMAKE_LIBRARY_SUFFIX "a")
  set(CMAKE_DLL_SUFFIXE "so")
  set(OS_ARCHITECH "Linux")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
