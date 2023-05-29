include_dep_dirs(
  ../

  ${PANGO_INCLUDE_DIRS}
  ${FONTCONFIG_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIRS}
  ${VLD_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIRS}
  ${VKVG_INCLUDE_DIRS}
  ${VULKAN_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${HARFBUZZ_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
  ${GLFW3_INCLUDE_DIRS}
  ${PIXMAN_INCLUDE_DIRS}
  ${GLIB_INCLUDE_DIRS}
  ${TINYEXPR_INCLUDE_DIRS}
  # ${MONO_INCLUDE_DIRS}
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

target_link_libraries(Mini
  CstCore
  FRFramework
  System
  ${PIXMAN_LIBRARIES}
  ${VLD_LIBRARIES}
  ${PTHREAD_LIBRARIES}
  ${VKVG_LIBRARIES}
  ${VULKAN_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${PANGO_LIBRARIES}
  ${HARFBUZZ_LIBRARIES}
  ${GLIB_LIBRARIES}
  ${GLFW3_LIBRARIES}
  ${M_LIBARIES}
  ${FREETYPE_LIBRARIES}
  ${FONTCONFIG_LIBRARIES}
  ${PNG_LIBRARIES}
  ${X11_LIBRARIES}
  ${PANGOCAIRO_LIBRARIES}
)

target_copy_files(Mini
  "${VLD_FILES}"
  "${PTHREAD_FILES}"
  "${VKVG_FILES}"
  "${VULKAN_FILES}"
  "${CAIRO_FILES}"
  "${GLFW3_FILES}"
  "${FREETYPE_FILES}"
  # "${MONO_FILES}"
)
