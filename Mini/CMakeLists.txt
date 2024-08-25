set(INC
  ../
  ${CMAKE_SOURCE_DIR}/../CstDemo/
)

set(EXTERNAL_LIBS
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
  "sdl3"
  "ffmpeg"
)

set(INNER_LIBS
  "System"
  "Framework"
  "CstCore"
)

set(SRC
  ./Front/MiniMenuBar.cst
  ./Front/MiniMenuBar.c
  ./Front/MiniMenuBar.h
  ./Front/MiniMenuBase.cst
  ./Front/MiniMenuBase.c
  ./Front/MiniMenuBase.h

  ./Front/MiniComponent.cst
  ./Front/MiniComponent.c
  ./Front/MiniComponent.h

  ./Runtime/MiniTypes.c
  ./Runtime/MiniTypes.h

  ./Mini.c
  ./Mini.h
)

add_executable(Mini ${SRC})
set_property(TARGET Mini PROPERTY FOLDER CstProject)

add_deps_N(Mini
  "${INC}"
  "${INNER_LIBS}"
  ""
  "${EXTERNAL_LIBS}"
)
