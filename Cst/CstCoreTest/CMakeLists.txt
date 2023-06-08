include_dep_dirs(
  ../

  ${GLIB_INCLUDE_DIRS}
  ${FONTCONFIG_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIRS}
  ${VLD_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIRS}
  ${VKVG_INCLUDE_DIRS}
  ${VULKAN_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
  ${GLFW3_INCLUDE_DIRS}
  ${PIXMAN_INCLUDE_DIRS}
  ${UNITY_INCLUDE_DIRS}
  ${MONO_INCLUDE_DIRS}
  ${TINYEXPR_INCLUDE_DIRS}
)

set(SRC
  ./TestConfig.h.in
  ./TestConfig.h
  ./TestCommon.h
  ./TestCore.c
  ./TestCore.h

  ./TestApp.c
  ./TestApp.h
  ./TestModule.c
  ./TestModule.h
  ./TestParser.c
  ./TestParser.h
  ./TestLayout.c
  ./TestLayout.h
  ./TestWindow.c
  ./TestWindow.h

  ./TestFramework.c
  ./TestFramework.h

  ./Front/AppButton.cst
  ./Front/AppComponent.cst
  ./Front/LayoutComponent.cst
  ./Front/LayoutRow.cst
  ./Front/LayoutBoxNested.cst
  ./Front/LayoutWrap.cst

  ./Front/AppButton.c
  ./Front/AppButton.h
  ./Front/AppComponent.c
  ./Front/AppComponent.h
  ./Front/LayoutComponent.c
  ./Front/LayoutComponent.h
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/TestConfig.h.in
               ${CMAKE_CURRENT_LIST_DIR}/TestConfig.h)


add_executable(CstCoreTest "${SRC}")
set_property(TARGET CstCoreTest PROPERTY FOLDER CstProject)

target_link_libraries(CstCoreTest
  CstCore
  System
  ${MONO_LIBRARIES}
  ${PIXMAN_LIBRARIES}
  ${VLD_LIBRARIES}
  ${PTHREAD_LIBRARIES}
  ${VKVG_LIBRARIES}
  ${VULKAN_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${SYSTEM_LIBRARIES}
  ${GLFW3_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${FONTCONFIG_LIBRARIES}
  ${PNG_LIBRARIES}
  ${UNITY_LIBRARIES}
  ${TINYEXPR_LIBRARIES}
  ${GLIB_LIBRARIES}
)

target_copy_files(CstCoreTest
  "${VLD_FILES}"
  "${CAIRO_FILES}"
  "${PANGO_FILES}"
  "${GLIB_FILES}"
  "${PTHREAD_FILES}"
  "${VKVG_FILES}"
  "${VULKAN_FILES}"
  # "${CAIRO_FILES}"
  "${GLFW3_FILES}"
  "${FREETYPE_FILES}"
  "${PNG_FILES}"
  "${PIXMAN_FILES}"
)
