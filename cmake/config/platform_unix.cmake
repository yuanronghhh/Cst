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
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(FREETYPE REQUIRED freetype2)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
pkg_check_modules(PANGOFC REQUIRED pangofc)
pkg_check_modules(FRIBID REQUIRED fribidi)
pkg_check_modules(EPOXY REQUIRED epoxy)
pkg_check_modules(GDK REQUIRED gdk-3.0)
pkg_check_modules(GDKWAYLAND REQUIRED gdk-wayland-3.0)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)

find_package(glad REQUIRED)
find_package(tinyexpr REQUIRED)
find_package(unity REQUIRED)

set(MPG123_LIBRARIES "mpg123")

set(GTK_LIBRARIES
  "/media/greyhound/Storage/Debian/gtk-4.6.3/_build/gtk/libgtk-4.so"
  "/media/greyhound/Storage/Git/glib/_build/glib/libglib-2.0.so"
  "/media/greyhound/Storage/Debian/gtk-4.6.3/_build/gdk/libgdk.a"
  "/media/greyhound/Storage/Git/glib/_build/gio/libgio-2.0.so"
  "/media/greyhound/Storage/Git/glib/_build/gobject/libgobject-2.0.so"
  # gtk-4
  pangocairo-1.0
  pango-1.0
  harfbuzz
  gdk_pixbuf-2.0
  cairo-gobject
  cairo
  graphene-1.0
  gio-2.0)

set(PTHREAD_LIBRARIES "-lpthread")
set(SYSTEM_LIBRARIES "-lexpat -lm -lrt -lrt -luuid")
