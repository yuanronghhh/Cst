set(CAIRO_GL 0 CACHE BOOL "" FORCE)
set(USE_MONO 0 CACHE BOOL "" FORCE)
set(USE_LLVM 1 CACHE BOOL "" FORCE)
set(USE_SANITIZER 0 CACHE BOOL "" FORCE)
set(USE_DEBUGGER 1 CACHE BOOL "" FORCE)

if(WIN32)
  set(OS_ARCHITECH "Win64")
  set(LIBDIR ${CMAKE_SOURCE_DIR}/../lib/${OS_ARCHITECH}_vc14)
  # set enviroment first

  if(USE_LLVM)
    SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../toolchain/ClangWindows.cmake")
    set(CMAKE_GENERATOR_TOOLSET "ClangCL")
  endif()

  if(USE_MONO)
    set(CMAKE_Csharp_COMPILER "C:/Program Files/Mono/bin/mcs.bat")
  endif()
else()
  set(OS_ARCHITECH "Linux")
  set(CLANG_HOME "/usr")
  set(CLANG_VERSION "18.1.4")
  set(LIBDIR ${CMAKE_SOURCE_DIR}/../lib/linux)
endif()

