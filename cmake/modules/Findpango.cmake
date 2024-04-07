set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

FIND_PATH(PANGO_INCLUDE_DIR
  NAMES pango/pangowin32.h
  HINTS ${search_dirs}
  PATH_SUFFIXES pango/include/pango-1.0
)

FIND_LIBRARY(PANGO_LIBRARY
  NAMES pango-1.0.lib
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib pango/lib
)

list(APPEND PANGO_LIBRARY
  "${LIBDIR}/pango/lib/pangocairo-1.0.lib"
  "${LIBDIR}/pango/lib/pangoft2-1.0.lib"
  "${LIBDIR}/pango/lib/pangowin32-1.0.lib"
)

set(PANGO_FILES "")
IF(WIN32)
  set(PANGO_FILE
    "pangoft2-1.0-0.dll"
    "pangocairo-1.0-0.dll"
    "pangowin32-1.0-0.dll"
    "pango-1.0-0.dll"
  )
  FOREACH(COMPONENT ${PANGO_FILE})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)


    FIND_FILE(PANGO_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES pango/bin
    )

    LIST(APPEND PANGO_FILES "${PANGO_${COMPONENT}_FILE}")
  ENDFOREACH()
ENDIF()

set(PANGO_DEPS
  cairo
  fontconfig
  freetype
  fribidi
  gettext
  glib
  harfbuzz
  vcpkg-tool-meson
)
add_dep_for_libray_N(pango "${PANGO_DEPS}")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PANGO DEFAULT_MSG
  PANGO_LIBRARY PANGO_INCLUDE_DIR)

IF(PANGO_FOUND)
  SET(PANGO_LIBRARIES ${PANGO_LIBRARY})
  SET(PANGO_INCLUDE_DIRS ${PANGO_INCLUDE_DIR})
ENDIF(PANGO_FOUND)

MARK_AS_ADVANCED(
  PANGO_INCLUDE_DIR
  PANGO_LIBRARY
)
