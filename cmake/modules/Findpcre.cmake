set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

FIND_PATH(PCRE_INCLUDE_DIR
  NAMES pcrecpp.h
  HINTS ${search_dirs}
  PATH_SUFFIXES pcre/include
)

FIND_LIBRARY(PCRE_LIBRARY
  NAMES pcreposix.lib
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib pcre/lib
)

set(PCRE_FILES "")
IF(WIN32)
  set(PCRE_FILE
    "pcreposix.dll"
    "pcre16.dll"
    "pcre.dll"
    "pcre32.dll"
    "pcrecpp.dll"
  )
  FOREACH(COMPONENT ${PCRE_FILE})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)


    FIND_FILE(PCRE_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES pcre/bin
    )

    LIST(APPEND PCRE_FILES "${PCRE_${COMPONENT}_FILE}")
  ENDFOREACH()
ENDIF()

set(PCRE_DEPS
)
add_dep_for_libray_N(pcre "${PCRE_DEPS}")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PCRE DEFAULT_MSG
  PCRE_LIBRARY PCRE_INCLUDE_DIR)

IF(PCRE_FOUND)
  SET(PCRE_LIBRARIES ${PCRE_LIBRARY})
  SET(PCRE_INCLUDE_DIRS ${PCRE_INCLUDE_DIR})
ENDIF(PCRE_FOUND)

MARK_AS_ADVANCED(
  PCRE_INCLUDE_DIR
  PCRE_LIBRARY
)

