set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

FIND_PATH(FFMPEG_INCLUDE_DIR
  NAMES libavutil/hash.h
  HINTS ${search_dirs}
  PATH_SUFFIXES ffmpeg/include
)

FIND_LIBRARY(FFMPEG_LIBRARY
  NAMES avcodec.lib
  HINTS ${search_dirs}
  PATH_SUFFIXES lib64 lib ffmpeg/lib
)

set(FFMPEG_FILES "")
IF(WIN32)
  set(FFMPEG_FILE
    "avformat-60.dll"
    "avcodec-60.dll"
    "avutil-58.dll"
    "avfilter-9.dll"
    "swresample-4.dll"
    "avdevice-60.dll"
    "swscale-7.dll"
  )
  FOREACH(COMPONENT ${FFMPEG_FILE})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(FFMPEG_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES ffmpeg/bin
    )

    LIST(APPEND FFMPEG_FILES "${FFMPEG_${COMPONENT}_FILE}")
  ENDFOREACH()

  LIST(APPEND FFMPEG_LIBRARY 
    "${LIBDIR}/ffmpeg/lib/avcodec.lib"
    "${LIBDIR}/ffmpeg/lib/avdevice.lib"
    "${LIBDIR}/ffmpeg/lib/avfilter.lib"
    "${LIBDIR}/ffmpeg/lib/avformat.lib"
    "${LIBDIR}/ffmpeg/lib/avutil.lib"
    "${LIBDIR}/ffmpeg/lib/swresample.lib"
    "${LIBDIR}/ffmpeg/lib/swscale.lib"
  )
ENDIF()

set(FFMPEG_DEPS
)
add_dep_for_libray_N(ffmpeg "${FFMPEG_DEPS}")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FFMPEG DEFAULT_MSG
  FFMPEG_LIBRARY FFMPEG_INCLUDE_DIR)

IF(FFMPEG_FOUND)
  SET(FFMPEG_LIBRARIES ${FFMPEG_LIBRARY})
  SET(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIR})
ENDIF(FFMPEG_FOUND)

MARK_AS_ADVANCED(
  FFMPEG_INCLUDE_DIR
  FFMPEG_LIBRARY
)

