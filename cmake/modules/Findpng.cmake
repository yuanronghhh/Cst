set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
  /usr/lib/x86_64-linux-gnu
)

FIND_PATH(PNG_INCLUDE_DIR
  NAMES png.h
  HINTS ${search_dirs}
  PATH_SUFFIXES libpng16/include include libpng/include/libpng16
)

FIND_LIBRARY(PNG_LIBRARY
  NAMES png16 libpng16
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib libpng/lib
)

set(PNG_FILES "")
if(WIN32)
  set(PNG_FILE_COMPONENTS
    "libpng16.dll"
    "zlib1.dll")

  FOREACH(COMPONENT ${PNG_FILE_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(PNG_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES libpng/bin
      )

    LIST(APPEND PNG_FILES "${PNG_${COMPONENT}_FILE}")
  ENDFOREACH()
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PNG DEFAULT_MSG
  PNG_LIBRARY PNG_INCLUDE_DIR)

IF(PNG_FOUND)
  SET(PNG_LIBRARIES ${PNG_LIBRARY})
  SET(PNG_INCLUDE_DIRS ${PNG_INCLUDE_DIR})
ENDIF(PNG_FOUND)

MARK_AS_ADVANCED(
  PNG_INCLUDE_DIR
  PNG_LIBRARY
  )

