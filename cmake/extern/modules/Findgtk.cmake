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
    "lzma.dll"
    "lzo2.dll"
    "zlib1.dll"
    "atk-1.0-0.dll"
    "tiff.dll"
    "jpeg62.dll"
    "epoxy-0.dll"
    "gdk_pixbuf-2.0-0.dll"
    "gio-2.0-0.dll"
    "glib-2.0-0.dll"
    "gmodule-2.0-0.dll"
    "gobject-2.0-0.dll"
    "graphene-1.0-0.dll"
    "gthread-2.0-0.dll"
    "gtk-4-1.dll"
    "intl-8.dll")

  FOREACH(COMPONENT ${GTK_FILE_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(GTK_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES gtk/bin gtk/tools/gtk)

    LIST(APPEND GTK_FILE "${GTK_${COMPONENT}_FILE}")
  ENDFOREACH()
endif()

LIST(APPEND GTK_INCLUDE_DIR "${CAIRO_INCLUDE_DIR}/cairo")
LIST(APPEND GTK_INCLUDE_DIR "${PANGO_INCLUDE_DIRS}")
LIST(APPEND GTK_INCLUDE_DIR "${PIXMAN_INCLUDE_DIRS}")
LIST(APPEND GTK_INCLUDE_DIR "${FREETYPE_INCLUDE_DIRS}")
LIST(APPEND GTK_INCLUDE_DIR "${PTHREAD_INCLUDE_DIRS}")
LIST(APPEND GTK_INCLUDE_DIR "${HARFBUZZ_INCLUDE_DIRS}")
LIST(APPEND GTK_INCLUDE_DIR "${LIBDIR}/gtk/include/gdk-pixbuf-2.0/")
LIST(APPEND GTK_INCLUDE_DIR "${LIBDIR}/gtk/include/graphene-1.0/")
LIST(APPEND GTK_INCLUDE_DIR "${LIBDIR}/gtk/include/gsk/")

LIST(APPEND GTK_FILE "${PANGO_FILES}")
LIST(APPEND GTK_FILE "${CAIRO_FILES}")

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

