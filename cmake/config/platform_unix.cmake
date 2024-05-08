set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  if(USE_SANITIZER)
    set(CMAKE_C_FLAGS "-Wall -g -fsanitize=address")
  else()
    set(CMAKE_C_FLAGS "-Wall -g -rdynamic")
    # set(CMAKE_C_FLAGS "-Wall -pg -rdynamic")
  endif()
else()
  set(CMAKE_C_FLAGS "-O3")
endif()

find_package(PkgConfig)

pkg_check_modules(X11 REQUIRED x11)
pkg_check_modules(CAIRO REQUIRED cairo;cairo-fc)
pkg_check_modules(PANGO REQUIRED pango)
pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(GLIB REQUIRED glib-2.0;gobject-2.0;gio-2.0)
pkg_check_modules(GTK REQUIRED gtk4;atk;fribidi;epoxy;gtk4-wayland)
pkg_check_modules(FREETYPE REQUIRED freetype2)
pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
pkg_check_modules(PANGOFC REQUIRED pangofc)
pkg_check_modules(FRIBID REQUIRED fribidi)
pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(EPOXY REQUIRED epoxy)
pkg_check_modules(GDK REQUIRED gdk-3.0;gdk-wayland-3.0)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(VULKAN REQUIRED vulkan)
pkg_check_modules(OPENSSL REQUIRED libssl;libcrypto)
pkg_check_modules(SSH2 REQUIRED libssh2)
pkg_check_modules(FFMPEG REQUIRED libavcodec;libavfilter;libavformat;libavutil;libswscale;libswresample)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0;gstreamer-video-1.0)

find_package(glad REQUIRED)
find_package(tinyexpr REQUIRED)
find_package(unity REQUIRED)
find_package(cglm REQUIRED)
find_package(system REQUIRED)

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
  gio-2.0
  atk-1.0
  epoxy
  xkbcommon
  gdk-3
  wayland-client
  wayland-egl
  gobject-2.0
  glib-2.0
  fribidi
  )

set(ADDTIONAL_LIBRARIES "-lpthread -lexpat -lm -lrt -lrt -luuid")
list(APPEND GTK_LIBRARIES ${ADDTIONAL_LIBRARIES})
