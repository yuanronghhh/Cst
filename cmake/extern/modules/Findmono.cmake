set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
  /usr/lib/x86_64-linux-gnu
)

FIND_PATH(MONO_INCLUDE_DIR
  NAMES mono/jit/jit.h
  HINTS ${search_dirs}
  PATH_SUFFIXES mono/include include mono/include/mono-2.0
)

set(MONO_FILES "")
set(MONO_FILE_COMPONENTS
  "libmono-btls-shared.dll"
  "mono-2.0-sgen.dll"
  "MonoPosixHelper.dll"
)

if(WIN32)
  FOREACH(COMPONENT ${MONO_FILE_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(MONO_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES mono/bin
    )

    LIST(APPEND MONO_FILES "${MONO_${COMPONENT}_FILE}")
  ENDFOREACH()
endif()

set(MONO_COMPONENTS
  mono-2.0-sgen
)

FOREACH(COMPONENT ${MONO_COMPONENTS})
  STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

  FIND_LIBRARY(MONO_${UPPERCOMPONENT}_LIBRARY
    NAMES ${COMPONENT}
    HINTS ${search_dirs}
    PATH_SUFFIXES mono/lib lib
  )

LIST(APPEND MONO_LIBRARY "${MONO_${UPPERCOMPONENT}_LIBRARY}")
ENDFOREACH()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MONO DEFAULT_MSG
  MONO_LIBRARY MONO_INCLUDE_DIR)

IF(MONO_FOUND)
  SET(MONO_LIBRARIES ${MONO_LIBRARY})
  SET(MONO_INCLUDE_DIRS ${MONO_INCLUDE_DIR})
ENDIF(MONO_FOUND)

MARK_AS_ADVANCED(
  MONO_INCLUDE_DIR
  MONO_LIBRARY
)
