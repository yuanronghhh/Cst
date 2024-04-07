set(search_dirs
  ${LIBDIR}
  /usr/local
  /usr
)

set(GSTREAMER_FILES "")
FIND_PATH(GSTREAMER_INCLUDE_DIR
  NAMES gst/gst.h
  HINTS ${search_dirs}
  PATH_SUFFIXES include/gstreamer gstreamer/include/gstreamer-1.0
)

if (WIN32)
  set(GSTREAMER_LIBS
    "gstallocators-1.0.lib"
    "gstapp-1.0.lib"
    "gstaudio-1.0.lib"
    "gstbase-1.0.lib"
    "gstcontroller-1.0.lib"
    "gstfft-1.0.lib"
    "gstgl-1.0.lib"
    "gstnet-1.0.lib"
    "gstpbutils-1.0.lib"
    "gstreamer-1.0/gstadder.lib"
    "gstreamer-1.0/gstapp.lib"
    "gstreamer-1.0/gstaudioconvert.lib"
    "gstreamer-1.0/gstaudiomixer.lib"
    "gstreamer-1.0/gstaudiorate.lib"
    "gstreamer-1.0/gstaudioresample.lib"
    "gstreamer-1.0/gstaudiotestsrc.lib"
    "gstreamer-1.0/gstcompositor.lib"
    "gstreamer-1.0/gstcoreelements.lib"
    "gstreamer-1.0/gstencoding.lib"
    "gstreamer-1.0/gstgio.lib"
    "gstreamer-1.0/gstopengl.lib"
    "gstreamer-1.0/gstoverlaycomposition.lib"
    "gstreamer-1.0/gstpbtypes.lib"
    "gstreamer-1.0/gstplayback.lib"
    "gstreamer-1.0/gstrawparse.lib"
    "gstreamer-1.0/gstsubparse.lib"
    "gstreamer-1.0/gsttcp.lib"
    "gstreamer-1.0/gsttypefindfunctions.lib"
    "gstreamer-1.0/gstvideoconvertscale.lib"
    "gstreamer-1.0/gstvideorate.lib"
    "gstreamer-1.0/gstvideotestsrc.lib"
    "gstreamer-1.0/gstvolume.lib"
    "gstreamer-1.0.lib"
    "gstriff-1.0.lib"
    "gstrtp-1.0.lib"
    "gstrtsp-1.0.lib"
    "gstsdp-1.0.lib"
    "gsttag-1.0.lib"
    "gstvideo-1.0.lib")

  FOREACH(COMPONENT ${GSTREAMER_LIBS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_LIBRARY(GSTREAMER_${UPPERCOMPONENT}_LIBRARY
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES lib64 lib gstreamer/lib
      )

    LIST(APPEND GSTREAMER_LIBRARY "${GSTREAMER_${UPPERCOMPONENT}_LIBRARY}")
  ENDFOREACH()
endif()

if(WIN32)
  set(GSTREAMER_FILE_COMPONENTS
    "gstallocators-1.0-0.dll"
    "gstapp-1.0-0.dll"
    "gstaudio-1.0-0.dll"
    "gstbase-1.0-0.dll"
    "gstcontroller-1.0-0.dll"
    "gstfft-1.0-0.dll"
    "gstgl-1.0-0.dll"
    "gstnet-1.0-0.dll"
    "gstpbutils-1.0-0.dll"
    "gstreamer-1.0-0.dll"
    "gstriff-1.0-0.dll"
    "gstrtp-1.0-0.dll"
    "gstrtsp-1.0-0.dll"
    "gstsdp-1.0-0.dll"
    "gsttag-1.0-0.dll"
    "gstvideo-1.0-0.dll")

  FOREACH(COMPONENT ${GSTREAMER_FILE_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)

    FIND_FILE(GSTREAMER_${COMPONENT}_FILE
      NAMES ${COMPONENT}
      HINTS ${search_dirs}
      PATH_SUFFIXES gstreamer/bin
    )

    LIST(APPEND GSTREAMER_FILES "${GSTREAMER_${COMPONENT}_FILE}")
  ENDFOREACH()
endif()

LIST(APPEND GSTREAMER_INCLUDE_DIR "${GSTREAMER_INCLUDE_DIR}/gstreamer")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GSTREAMER DEFAULT_MSG
  GSTREAMER_LIBRARY GSTREAMER_INCLUDE_DIR)

IF(GSTREAMER_FOUND)
  SET(GSTREAMER_LIBRARIES ${GSTREAMER_LIBRARY})
  SET(GSTREAMER_INCLUDE_DIRS ${GSTREAMER_INCLUDE_DIR})
ENDIF(GSTREAMER_FOUND)

MARK_AS_ADVANCED(
  GSTREAMER_INCLUDE_DIR
  GSTREAMER_LIBRARY
)
