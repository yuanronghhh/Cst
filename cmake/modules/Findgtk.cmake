set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

FIND_PATH(GTK_INCLUDE_DIR
  NAMES gtk/gtk.h
  HINTS ${search_dirs}
  PATH_SUFFIXES include/gtk-4.0 include gtk/include/gtk-4.0)

FIND_LIBRARY(GTK_LIBRARY
  NAMES gtk-4 gtk
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib gtk/lib)

if(WIN32)
  set(GTK_FILE_COMPONENTS
    "gtk-4-1.dll"
  )

  FOREACH(COMPONENT ${GTK_FILE_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(GTK_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES gtk/bin gtk/tools/gtk)

    LIST(APPEND GTK_FILE "${GTK_${COMPONENT}_FILE}")
  ENDFOREACH()
endif()

set(GTK_DEPS
  "brotli"
  "lzo"
  "lzma"
  "pixman"
  "pcre2"
  "jpegturbo"
  "tiff"
  "png"
  "pango"
  "libepoxy"
  "glib"
  "cairo"
  "pixman"
  "graphene"
  "freetype"
  "pthread"
  "harfbuzz"
  "gdkpixbuf"
)
add_dep_for_libray_N(gtk "${GTK_DEPS}")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTK DEFAULT_MSG
  GTK_LIBRARY GTK_INCLUDE_DIR)

IF(GTK_FOUND)
  SET(GTK_LIBRARIES ${GTK_LIBRARY})
  SET(GTK_INCLUDE_DIRS ${GTK_INCLUDE_DIR})
  SET(GTK_FILES ${GTK_FILE})
ENDIF(GTK_FOUND)

MARK_AS_ADVANCED(
  GTK_INCLUDE_DIR
  GTK_LIBRARY
)

