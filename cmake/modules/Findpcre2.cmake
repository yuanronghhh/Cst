set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

FIND_PATH(PCRE2_INCLUDE_DIR
  NAMES pcre2.h
  HINTS ${search_dirs}
  PATH_SUFFIXES pcre2/include
)

FIND_LIBRARY(PCRE2_LIBRARY
  NAMES pcre2-posix.lib
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib pcre2/lib
)

set(PCRE2_FILES "")
IF(WIN32)
  set(PCRE2_FILE
    "pcre2-8.dll"
    "pcre2-posix.dll"
    "pcre2-16.dll"
    "pcre2-32.dll"
  )
  FOREACH(COMPONENT ${PCRE2_FILE})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)


    FIND_FILE(PCRE2_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES pcre2/bin
    )

    LIST(APPEND PCRE2_FILES "${PCRE2_${COMPONENT}_FILE}")
  ENDFOREACH()
ENDIF()

set(PCRE2_DEPS
  bzip2
  vcpkg-cmake
  vcpkg-cmake-config
  zlib
)
add_dep_for_libray_N(pcre2 "${PCRE2_DEPS}")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PCRE2 DEFAULT_MSG
  PCRE2_LIBRARY PCRE2_INCLUDE_DIR)

IF(PCRE2_FOUND)
  SET(PCRE2_LIBRARIES ${PCRE2_LIBRARY})
  SET(PCRE2_INCLUDE_DIRS ${PCRE2_INCLUDE_DIR})
ENDIF(PCRE2_FOUND)

MARK_AS_ADVANCED(
  PCRE2_INCLUDE_DIR
  PCRE2_LIBRARY
)

