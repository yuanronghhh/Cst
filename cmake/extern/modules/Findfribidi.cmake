set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
  /usr/lib/x86_64-linux-gnu
)

set(FRIBIDI_FILES "")
LIST(APPEND FRIBIDI_FILES "${HARFBUZZ_FILES}")
LIST(REMOVE_DUPLICATES FRIBIDI_FILES)

FIND_PATH(FRIBIDI_INCLUDE_DIR
  NAMES fribidi/fribidi.h
  HINTS ${search_dirs}
  PATH_SUFFIXES include fribidi/include)

FIND_LIBRARY(FRIBIDI_LIBRARY
  NAMES fribidi
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib fribidi/lib)
LIST(APPEND FRIBIDI_LIBRARY "${FRIBIDI_LIBRARY}")

if(WIN32)
  FIND_FILE(FRIBIDI_FILE
    NAMES "fribidi-0.dll"
    HINTS ${search_dirs}
    PATH_SUFFIXES fribidi/bin)

  LIST(APPEND FRIBIDI_FILES "${FRIBIDI_FILE}")
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FRIBIDI DEFAULT_MSG
  FRIBIDI_LIBRARY FRIBIDI_INCLUDE_DIR)

IF(FRIBIDI_FOUND)
  SET(FRIBIDI_LIBRARIES ${FRIBIDI_LIBRARY})
  SET(FRIBIDI_INCLUDE_DIRS ${FRIBIDI_INCLUDE_DIR})
ENDIF(FRIBIDI_FOUND)

MARK_AS_ADVANCED(
  FRIBIDI_INCLUDE_DIR
  FRIBIDI_LIBRARY
)
