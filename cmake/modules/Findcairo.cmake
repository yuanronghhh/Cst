set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

FIND_PATH(CAIRO_INCLUDE_DIR
  NAMES cairo/cairo.h cairo.h
  HINTS ${search_dirs}
  PATH_SUFFIXES cairo/include
)
list(APPEND CAIRO_INCLUDE_DIR
  "${CAIRO_INCLUDE_DIR}/cairo"
)

FIND_LIBRARY(CAIRO_LIBRARY
  NAMES cairo.lib
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib cairo/lib
)

set(CAIRO_FILES "")
IF(WIN32)
  set(CAIRO_FILE
    "cairo-2.dll"
    "cairo-script-interpreter-2.dll"
    "cairo-gobject-2.dll"
  )

  FOREACH(COMPONENT ${CAIRO_FILE})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)


    FIND_FILE(CAIRO_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES cairo/bin
    )

    LIST(APPEND CAIRO_FILES "${CAIRO_${COMPONENT}_FILE}")
  ENDFOREACH()
ENDIF()

set(CAIRO_DEPS
  brotli
  freetype
  dirent
  expat
  png
  lzo
  pixman
  pthread
  zlib
  bzip2
)
add_dep_for_libray_N(cairo "${CAIRO_DEPS}")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CAIRO DEFAULT_MSG
  CAIRO_LIBRARY CAIRO_INCLUDE_DIR)

IF(CAIRO_FOUND)
  SET(CAIRO_LIBRARIES ${CAIRO_LIBRARY})
  SET(CAIRO_INCLUDE_DIRS ${CAIRO_INCLUDE_DIR})
ENDIF(CAIRO_FOUND)

MARK_AS_ADVANCED(
  CAIRO_INCLUDE_DIR
  CAIRO_LIBRARY
)

