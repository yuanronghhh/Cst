SET(INC
  ../

  ${VKVG_INCLUDE_DIRS}
  ${VLD_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
  ${GLFW3_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${FONTCONFIG_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${PIXMAN_INCLUDE_DIRS}
  ${MONO_INCLUDE_DIRS}
  ${TINYEXPR_INCLUDE_DIRS}
)

set(SRC
  ./CstCli.c
  ./CstCli.h

  ../CstCore/Parser/CstCli.l
  ../CstCore/Parser/CstCli.y
)

include_dep_dirs(${INC})
add_executable(CstCli "${SRC}")
set_property(TARGET CstCli PROPERTY FOLDER CstProject)

add_dependencies(CstCli
  CstCore
)

target_link_libraries(CstCli
  CstCore
  System
  ${MONO_LIBRARIES}
  ${PIXMAN_LIBRARIES}
  ${FONTCONFIG_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${VLD_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${PTHREAD_LIBRARY}
  ${VKVG_LIBRARIES}
  ${GLFW3_LIBRARIES}
)
