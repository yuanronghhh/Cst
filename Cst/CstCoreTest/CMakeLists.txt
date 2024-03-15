set(CST_TEST_INC
  ../
)

set(CST_TEST_EXTERNAL_LIBS
  "unity"
  "pthread"
  "openssl"
  "vld"
  "pango"
  "pangocairo"
  "cairo"
  "glad"
  "glfw3"
)

set(CST_TEST_INNER_LIBS
  "FRFramework"
  "System"
  "CstCore"
)

set(SRC
  ./TestConfig.h.in
  ./TestConfig.h
  ./TestCommon.h
  ./TestCore.c
  ./TestCore.h

  ./TestBasic.c
  ./TestBasic.h
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
  ./Front/AppButton.c
  ./Front/AppButton.h
  ./Front/LayoutComponent.cst
  ./Front/LayoutComponent.c
  ./Front/LayoutComponent.h
  ./Front/AppBox.cst
  ./Front/AppBox.c
  ./Front/AppBox.h
  ./Front/AppComponent.cst
  ./Front/AppComponent.c
  ./Front/AppComponent.h

  ./Front/LayoutRow.cst
  ./Front/LayoutBoxNested.cst
  ./Front/LayoutWrap.cst
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/TestConfig.h.in
               ${CMAKE_CURRENT_LIST_DIR}/TestConfig.h)

add_executable(CstCoreTest "${SRC}")
set_property(TARGET CstCoreTest PROPERTY FOLDER CstProject)

LIST(APPEND CST_TEST_INNER_LIBS ${SYSTEM_LIBRARIES})
log("${PANGO_LIBRARIES}")
add_deps_N(CstCoreTest
  "${CST_TEST_INC}"
  "${CST_TEST_INNER_LIBS}"
  ""
  "${CST_TEST_EXTERNAL_LIBS}"
)
