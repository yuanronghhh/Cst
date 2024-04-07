set(INC
  ../
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
  "system"
  "pangofc"
  "tinyexpr"
)

set(INNER_LIBS
  "FrFramework"
  "CstCore"
)

set(SRC
  ./CstCli.c
  ./CstCli.h

  ../CstCore/Parser/CstCli.l
  ../CstCore/Parser/CstCli.y
)

include_dep_dirs(${INC})
add_executable(CstCli "${SRC}")
add_deps_N(CstCli
  "${INC}"
  "${INNER_LIBS}"
  ""
  "${EXTERNAL_LIBS}"
)
set_property(TARGET CstCli PROPERTY FOLDER CstProject)

add_dependencies(CstCli
  ${INNER_LIBS}
)
