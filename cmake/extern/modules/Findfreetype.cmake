set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
  /usr/lib/x86_64-linux-gnu
)

FIND_PATH(FREETYPE_INCLUDE_DIR
  NAMES ft2build.h freetype/freetype.h
  HINTS ${search_dirs}
  PATH_SUFFIXES freetype/include include/freetype2
)

FIND_LIBRARY(FREETYPE_LIBRARY
  NAMES freetype
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib freetype/lib
)

if (UNIX)
LIST(APPEND FREETYPE_LIBRARY
  "brotlidec"
  "fontconfig"
  "z"
)
endif()

set(FREETYPE_FILE_COMPONENTS
  "freetype.dll"
)

set(FREETYPE_FILE "")
if(WIN32)
  FOREACH(COMPONENT ${FREETYPE_FILE_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(FREETYPE_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES freetype/bin
    )

    LIST(APPEND FREETYPE_FILE "${FREETYPE_${COMPONENT}_FILE}")
  ENDFOREACH()
endif()

LIST(APPEND FREETYPE_INCLUDE_DIR "${FONTCONFIG_INCLUDE_DIRS}")
LIST(APPEND FREETYPE_FILE "${FONTCONFIG_FILES}")
LIST(REMOVE_DUPLICATES FREETYPE_FILE)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREETYPE DEFAULT_MSG
  FREETYPE_LIBRARY FREETYPE_INCLUDE_DIR)

IF(FREETYPE_FOUND)
  SET(FREETYPE_LIBRARIES ${FREETYPE_LIBRARY})
  SET(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR})
  SET(FREETYPE_FILES ${FREETYPE_FILE})
ENDIF(FREETYPE_FOUND)

MARK_AS_ADVANCED(
  FREETYPE_INCLUDE_DIR
  FREETYPE_LIBRARY
  FREETYPE_FILES
)
