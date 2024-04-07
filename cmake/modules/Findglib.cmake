set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

FIND_PATH(GLIB_INCLUDE_DIR
  NAMES glib.h
  HINTS ${search_dirs}
  PATH_SUFFIXES glib/include/glib-2.0
)

FIND_LIBRARY(GLIB_LIBRARY
  NAMES glib-2.0.lib
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib glib/lib
)

list(APPEND GLIB_LIBRARY
  "${LIBDIR}/glib/lib/gio-2.0.lib"
  "${LIBDIR}/glib/lib/gmodule-2.0.lib"
  "${LIBDIR}/glib/lib/gobject-2.0.lib"
  "${LIBDIR}/glib/lib/gthread-2.0.lib"
)

set(GLIB_FILES "")
IF(WIN32)
  set(GLIB_FILE
    "gobject-2.0-0.dll"
    "gthread-2.0-0.dll"
    "gio-2.0-0.dll"
    "gmodule-2.0-0.dll"
    "glib-2.0-0.dll"
  )
  FOREACH(COMPONENT ${GLIB_FILE})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)


    FIND_FILE(GLIB_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES glib/bin
    )

    LIST(APPEND GLIB_FILES "${GLIB_${COMPONENT}_FILE}")
  ENDFOREACH()
ENDIF()

set(GLIB_DEPS
  dirent
  gettext
  libffi
  libiconv
  pcre2
  zlib
)
add_dep_for_libray_N(glib "${GLIB_DEPS}")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLIB DEFAULT_MSG
  GLIB_LIBRARY GLIB_INCLUDE_DIR)

IF(GLIB_FOUND)
  SET(GLIB_LIBRARIES ${GLIB_LIBRARY})
  SET(GLIB_INCLUDE_DIRS ${GLIB_INCLUDE_DIR})
ENDIF(GLIB_FOUND)

MARK_AS_ADVANCED(
  GLIB_INCLUDE_DIR
  GLIB_LIBRARY
)

