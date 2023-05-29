set(OS_ARCHITECH "Linux")
set(LIBDIR ${CMAKE_SOURCE_DIR}/../lib/linux)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(CMAKE_C_FLAGS "-Wall -g -rdynamic")
else()
  set(CMAKE_C_FLAGS "-Os -fvisibility=hidden")
endif()

find_package(PkgConfig)

pkg_check_modules(X11 REQUIRED x11)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PANGO REQUIRED pango)
pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)
pkg_check_modules(GLFW3 REQUIRED glfw3)
find_package(glad REQUIRED)
find_package(tinyexpr REQUIRED)

set(MPG123_LIBRARIES "mpg123")

set(GLIB_LIBRARIES
  "/usr/lib/x86_64-linux-gnu/libgio-2.0.so"
  "/usr/lib/x86_64-linux-gnu/libglib-2.0.so"
  "/usr/lib/x86_64-linux-gnu/libgmodule-2.0.so"
  "/usr/lib/x86_64-linux-gnu/libgobject-2.0.so"
  "/usr/lib/x86_64-linux-gnu/libgthread-2.0.so"
)

set(PTHREAD_LIBRARIES "-lpthread")
set(SYSTEM_LIBRARIES "-lexpat -lm -lrt -lrt -luuid")
