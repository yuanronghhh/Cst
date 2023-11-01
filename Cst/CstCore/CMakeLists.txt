set(INC
  ../

  ${GLIB_INCLUDE_DIRS}
  ${PANGO_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${VLD_INCLUDE_DIRS}
  ${GLFW3_INCLUDE_DIRS}
  ${UNITY_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIRS}
)

set(INC_SYS
)


set(C_API_SRC
  ./Front/CstType.h
  ./Front/Common/CstComNode.c
  ./Front/Common/CstComNode.h
  ./Front/Common/CstComNodeBuilder.c
  ./Front/Common/CstComNodeBuilder.h

  ./Front/Common/CstLBox.c
  ./Front/Common/CstLBox.h
  ./Front/Common/CstLGrid.c
  ./Front/Common/CstLGrid.h
  ./Front/Common/CstLBody.c
  ./Front/Common/CstLBody.h
  ./Front/Common/CstText.c
  ./Front/Common/CstText.h
  ./Front/Common/CstComRNode.h
  ./Front/Common/CstComRNode.c

  ./Front/Common/CstLGridContext.c
  ./Front/Common/CstLGridContext.h
  ./Front/Common/CstLBoxContext.c
  ./Front/Common/CstLBoxContext.h
  ./Front/Common/CstLBodyContext.c
  ./Front/Common/CstLBodyContext.h
)

set(SRC
  ./BuildIn/Components/CstLabel.h
  ./BuildIn/Components/CstLabel.c
  ./BuildIn/Components/CstLabel.cst

  ./Driver/CstTypes.h
  ./Driver/CstCommon.c
  ./Driver/CstCommon.h

  ./Driver/CstApplication.c
  ./Driver/CstApplication.h
  ./Driver/CstContext.c
  ./Driver/CstContext.h
  ./Driver/CstRender.c
  ./Driver/CstRender.h

  ./Driver/CstLayer.c
  ./Driver/CstLayer.h
  ./Driver/CstLayerNode.c
  ./Driver/CstLayerNode.h
  ./Driver/CstBoxLayer.c
  ./Driver/CstBoxLayer.h
  ./Driver/CstAbsLayer.c
  ./Driver/CstAbsLayer.h

  ./Driver/CstModule.c
  ./Driver/CstModule.h
  ./Driver/CstNode.c
  ./Driver/CstNode.h
  ./Driver/CstNodeBuilder.c
  ./Driver/CstNodeBuilder.h

  ./Driver/Css/CstCssGroup.c
  ./Driver/Css/CstCssGroup.h
  ./Driver/Css/CstCssPair.c
  ./Driver/Css/CstCssPair.h
  ./Driver/Css/CstCssNode.c
  ./Driver/Css/CstCssNode.h
  ./Driver/Css/CstCssClosure.c
  ./Driver/Css/CstCssClosure.h
  ./Driver/Css/CstCssEnv.c
  ./Driver/Css/CstCssEnv.h
  ./Driver/Css/CstCss.c
  ./Driver/Css/CstCss.h

  ./Driver/Css/CstCssValue.c
  ./Driver/Css/CstCssValue.h
  ./Driver/Css/CstCssParser.c
  ./Driver/Css/CstCssParser.h

  ./Driver/Css/CstCssCore.h

  ./Driver/CstLayout.h
  ./Driver/CstLayout.c
  ./Driver/CstLayoutNode.h
  ./Driver/CstLayoutNode.c
  ./Driver/CstRenderContext.h
  ./Driver/CstRenderContext.c
  ./Driver/CstRenderNode.h
  ./Driver/CstRenderNode.c
  ./Driver/CstBoxNode.h
  ./Driver/CstBoxNode.c
  ./Driver/CstRow.h
  ./Driver/CstRow.c
  ./Driver/CstNodeMap.h
  ./Driver/CstNodeMap.c
  ./Driver/CstValueMap.h
  ./Driver/CstValueMap.c

  ./Driver/CstComponentBuilder.c
  ./Driver/CstComponentBuilder.h
  ./Driver/CstComponent.h
  ./Driver/CstComponent.c

  ./Front/CstFrontCore.h

  ./Parser/CstCli.l
  ./Parser/CstCli.y
  ./Parser/CstCli.yy.h
  ./Parser/CstCli.yy.c
  ./Parser/lex.yy.c

  ./Parser/Ast.c
  ./Parser/Ast.h
  # ./Parser/CodeGen.c
  # ./Parser/CodeGen.h

  ./Parser/CstParser.c
  ./Parser/CstParser.h
  ./Parser/CstParserCommon.h
  ./Parser/CstParserTypes.h
  ./Parser/CstParserCore.h

  ./Config/CstConfig.h.in
  ./CstConfig.h
  ./CstCore.h
)

if(USE_MONO)
else()
    LIST(APPEND SRC ${C_API_SRC})
endif()

add_custom_target(CstCliGen
  COMMAND bison -o ./Cst/CstCore/Parser/CstCli.yy.c -d ./Cst/CstCore/Parser/CstCli.y && flex -o ./Cst/CstCore/Parser/lex.yy.c ./Cst/CstCore/Parser/CstCli.l
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_property(TARGET CstCliGen PROPERTY FOLDER CstProject)

add_dep_libs(CstCore "${SRC}" "${INC}" "${INC_SYS}")

add_dependencies(CstCore
  FRFramework
  System
)

target_link_libraries(CstCore
  FRFramework
  System
  ${PANGO_LIBRARIES}
  ${GLIB_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)
set_property(TARGET CstCore PROPERTY FOLDER CstProject)
configure_file(${CMAKE_CURRENT_LIST_DIR}/Config/CstConfig.h.in
  ${CMAKE_CURRENT_LIST_DIR}/CstConfig.h)

# CstCMake
set(CMakeFile
  ../CMakeLists.txt
  ../../cmake/init.cmake
  ../../cmake/macros/logger.cmake
  ../../cmake/macros/policy.cmake
  ../../cmake/macros/project_common.cmake
  ../../cmake/config/build_option.cmake
  ../../cmake/config/platform_unix.cmake
  ../../cmake/config/platform_win32.cmake
  ../../release/icon/cst.rc
  ../../CMakeLists.txt
)

add_library(CstCMake OBJECT ${CMakeFile})
set_property(TARGET CstCMake PROPERTY FOLDER CstProject)

# if(WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL "WIN32")
#   set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "/wd4819")
#   set_target_properties(${EXECUTABLE_NAME} PROPERTIES WIN32_EXECUTEABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup")
# endif()
