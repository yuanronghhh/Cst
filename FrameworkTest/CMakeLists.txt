set(INC
  ../
  ./tests/
  ${INIT_HOME}/../
  # ${CMAKE_SOURCE_DIR}/../CstDemo/
)

set(EXTERNAL_LIBS
  "System"
  "Unity"
  "pthread"
  "openssl"
  "vld"
  "pango"
  "pangocairo"
  "cairo"
  "glib"
  "glad"
  "glfw3"
  "sdl3"
  "pangofc"
  "tinyexpr"
  "ffmpeg"
)

set(INNER_LIBS
  "Framework"
)

set(SRC
  ./tests/TestVideo.c
  ./tests/TestVideo.h
  ./tests/TestJob.c
  ./tests/TestJob.h

  ./TestCore.c
  ./TestCore.h
  ./Common.h
  ./FrTestConfig.h
  ./FrTestConfig.h.in
)

# set(FFMPEG_LIBRARIES
#   "/media/greyhound/Storage/Git/FFmpeg/libavcodec/libavcodec.a"
#   "/media/greyhound/Storage/Git/FFmpeg/libavfilter/libavfilter.a"
#   "/media/greyhound/Storage/Git/FFmpeg/libavformat/libavformat.a"
#   "/media/greyhound/Storage/Git/FFmpeg/libavutil/libavutil.a"
#   "/media/greyhound/Storage/Git/FFmpeg/libswscale/libswscale.a"
#   "/media/greyhound/Storage/Git/FFmpeg/libswresample/libswresample.a"
# )

add_executable(FrameworkTest "${SRC}")
LIST(APPEND EXTERNAL_LIBS "${ADDTIONAL_LIBRARIES}")

add_deps_options_N(FrameworkTest
  "${INC}"
  "${INNER_LIBS}"
  ""
  "${EXTERNAL_LIBS}"
  "${ADDTIONAL_LIBRARIES}"
)
set_property(TARGET FrameworkTest PROPERTY FOLDER CstProject)
configure_file(${CMAKE_CURRENT_LIST_DIR}/FrTestConfig.h.in
  ${CMAKE_CURRENT_LIST_DIR}/FrTestConfig.h)
# set_target_properties(FrameworkTest PROPERTIES LINK_FLAGS "/PROFILE")
