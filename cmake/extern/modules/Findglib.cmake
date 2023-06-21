set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
  /usr/lib/x86_64-linux-gnu
)

set(GLIB_FILES "")
FIND_PATH(GLIB_INCLUDE_DIR
  NAMES glib.h
  HINTS ${search_dirs}
  PATH_SUFFIXES include glib/include glib/include/glib-2.0
)

LIST(APPEND GLIB_INCLUDE_DIR
  "${LIBDIR}/glib/include/glib-2.0/gio"
  "${LIBDIR}/glib/include/glib-2.0/gmodule"
  "${LIBDIR}/glib/include/glib-2.0/glib"
  "${LIBDIR}/glib/include/glib-2.0/gobject"
)

if(UNIX)
  LIST(APPEND GLIB_INCLUDE_DIR
    "/usr/lib/x86_64-linux-gnu/glib-2.0/include"
  )
endif()

set(GLIB_COMPONENTS
  gio-2.0
  glib-2.0
  gmodule-2.0
  gobject-2.0
  gthread-2.0
)


if(WIN32)
  # LIST(APPEND GLIB_COMPONENTS
  #   iconv
  #   intl
  #   libffi
  #   pcre
  #   pcre16
  #   pcre32
  #   pcrecpp
  #   pcreposix
  #   zlib
  # )

set(GLIB_FILE
  "brotlicommon.dll"
  "brotlidec.dll"
  # "brotlienc.dll"
  "bz2.dll"
  "charset-1.dll"

  "gio-2.0-0.dll"
  "glib-2.0-0.dll"
  "gmodule-2.0-0.dll"
  "gobject-2.0-0.dll"
  "gthread-2.0-0.dll"

  "iconv-2.dll"
  "intl-8.dll"
  "libexpat.dll"
  "libffi.dll"
  "harfbuzz.dll"
  "pcre2-8.dll"
  "pcre.dll"
  "pcre16.dll"
  "pcre32.dll"
  "pcrecpp.dll"
  "pcreposix.dll"
  "zlib1.dll")

FOREACH(COMPONENT ${GLIB_FILE})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(GLIB_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES glib/bin pango/tools/pango fontconfig/tools/fontconfig/ bzip2/bin libiconv/bin glib/tools/glib expat/bin/ pcre/bin
    )

    LIST(APPEND GLIB_FILES "${GLIB_${COMPONENT}_FILE}")
  ENDFOREACH()
endif()

FOREACH(COMPONENT ${GLIB_COMPONENTS})
  STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

  FIND_LIBRARY(GLIB_${COMPONENT}_LIBRARY
    NAMES ${COMPONENT}
    HINTS ${search_dirs}
    PATH_SUFFIXES lib64 lib glib/lib
  )

  LIST(APPEND GLIB_LIBRARY "${GLIB_${COMPONENT}_LIBRARY}")
ENDFOREACH()

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
