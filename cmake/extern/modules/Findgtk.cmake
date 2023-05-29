set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

set(GTK_COMPONENTS
  atk-1.0
  cairo-gobject
  cairo-script-interpreter
  cairo
  epoxy
  gdk_pixbuf-2.0
  gio-2.0
  glib-2.0
  gmodule-2.0
  gobject-2.0
  graphene-1.0
  gthread-2.0
  gtk-4
  harfbuzz-subset
  harfbuzz
  intl
  pango-1.0
  pangocairo-1.0
  pangoft2-1.0
  pangowin32-1.0
)

FIND_PATH(GTK_INCLUDE_DIR
  NAMES gtk/gtk.h
  HINTS ${search_dirs}
  PATH_SUFFIXES include/gtk-4.0 include
)

FOREACH(COMPONENT ${GTK_COMPONENTS})
  STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

  FIND_LIBRARY(GTK_${UPPERCOMPONENT}_LIBRARY
    NAMES ${COMPONENT}
    HINTS ${search_dirs}
    PATH_SUFFIXES lib64 lib
  )

  LIST(APPEND GTK_LIBRARY "${GTK_${UPPERCOMPONENT}_LIBRARY}")
ENDFOREACH()

LIST(APPEND GTK_INCLUDE_DIR
  "${GTK_INCLUDE_DIR}/../atk-1.0"
  "${GTK_INCLUDE_DIR}/../cairo"
  "${GTK_INCLUDE_DIR}/../epoxy"
  "${GTK_INCLUDE_DIR}/../gdk"
  "${GTK_INCLUDE_DIR}/../gdk-pixbuf-2.0"
  "${GTK_INCLUDE_DIR}/../gio-win32-2.0"
  "${GTK_INCLUDE_DIR}/../glib-2.0"
  "${GTK_INCLUDE_DIR}/../graphene-1.0"
  "${GTK_INCLUDE_DIR}/../gsk"
  "${GTK_INCLUDE_DIR}/../gtk-4.0"
  "${GTK_INCLUDE_DIR}/../harfbuzz"
  "${GTK_INCLUDE_DIR}/../pango-1.0"
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTK DEFAULT_MSG
  GTK_LIBRARY GTK_INCLUDE_DIR)

IF(GTK_FOUND)
  SET(GTK_LIBRARIES ${GTK_LIBRARY})
  SET(GTK_INCLUDE_DIRS ${GTK_INCLUDE_DIR})
ENDIF(GTK_FOUND)

MARK_AS_ADVANCED(
  GTK_INCLUDE_DIR
  GTK_LIBRARY
)

