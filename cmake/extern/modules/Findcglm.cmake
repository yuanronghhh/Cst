set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
  /usr/lib/x86_64-linux-gnu
)

FIND_PATH(CGLM_INCLUDE_DIR
  NAMES cglm/cglm.h
  HINTS ${search_dirs}
  PATH_SUFFIXES cglm/include
)

FIND_LIBRARY(CGLM_LIBRARY
  NAMES cglm
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib cglm/lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CGLM DEFAULT_MSG
  CGLM_LIBRARY CGLM_INCLUDE_DIR)

IF(CGLM_FOUND)
  SET(CGLM_LIBRARIES ${CGLM_LIBRARY})
  SET(CGLM_INCLUDE_DIRS ${CGLM_INCLUDE_DIR})
ENDIF(CGLM_FOUND)

MARK_AS_ADVANCED(
  CGLM_INCLUDE_DIR
  CGLM_LIBRARY
)
