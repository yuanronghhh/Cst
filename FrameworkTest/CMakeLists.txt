set(EXTERNAL_LIBS
  "System"
  "unity"
  "pthread"
  "openssl"
  "vld"
  "pango"
  "pangocairo"
  "cairo"
  "glib"
  "glad"
  "glfw3"
  "pangofc"
  "tinyexpr"
  "ffmpeg"
)

set(INNER_LIBS
  "Framework"
)

set(INC
  ../
  ./tests/
  ${INIT_HOME}/../
  ${CMAKE_SOURCE_DIR}/../CstDemo/
)

set(SRC
  ./tests/TestVideo.c
  ./tests/TestVideo.h

  ./TestCore.c
  ./TestCore.h
  ./Common.h
  ./FrTestConfig.h
  ./FrTestConfig.h.in
)

add_executable(FrameworkTest "${SRC}")

add_deps_N(FrameworkTest
  "${INC}"
  "${INNER_LIBS}"
  ""
  "${EXTERNAL_LIBS}"
)
set_property(TARGET FrameworkTest PROPERTY FOLDER CstProject)
configure_file(${CMAKE_CURRENT_LIST_DIR}/FrTestConfig.h.in
  ${CMAKE_CURRENT_LIST_DIR}/FrTestConfig.h)
