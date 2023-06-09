set(OS_ARCHITECH "Msys")
set(LIBDIR ${CMAKE_SOURCE_DIR}/../lib/${OS_ARCHITECH})

if(NOT EXISTS "${LIBDIR}/")
  message(FATAL_ERROR "Windows requires pre-compiled libs at: '${LIBDIR}'")
endif()

file(GLOB children RELATIVE ${LIBDIR} ${LIBDIR}/*)
foreach(child ${children})
  if(IS_DIRECTORY ${LIBDIR}/${child})
    list(APPEND CMAKE_PREFIX_PATH ${LIBDIR}/${child})
  endif()
endforeach()

find_package(unity REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glib REQUIRED)
find_package(gcoroutine REQUIRED)
find_package(stb REQUIRED)
find_package(fontconfig REQUIRED)
find_package(freetype REQUIRED)
find_package(harfbuzz REQUIRED)
find_package(cglm REQUIRED)
find_package(tinyexpr REQUIRED)
find_package(glad REQUIRED)
find_package(vulkan REQUIRED)
find_package(vkvg REQUIRED)
find_package(mono REQUIRED)
find_package(clang REQUIRED)
find_package(llvm REQUIRED)
find_package(czmq REQUIRED)
find_package(nanomsg REQUIRED)
find_package(gtk REQUIRED)
find_package(pixman REQUIRED)
find_package(curl REQUIRED)
find_package(pthread REQUIRED)
find_package(vld REQUIRED)
find_package(bdwgc REQUIRED)
find_package(cairo REQUIRED)
