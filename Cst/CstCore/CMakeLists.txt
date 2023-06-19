set(INC
  ../

  ${GLIB_INCLUDE_DIRS}
  ${PANGO_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${VLD_INCLUDE_DIRS}
  ${GLFW3_INCLUDE_DIRS}
  ${UNITY_INCLUDE_DIRS}
  ${TINYEXPR_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
)

set(INC_SYS
)


set(MONO_API_SRC
  ./Front/Mono/CstMType.h

  ./Front/Mono/CstMNode.c
  ./Front/Mono/CstMNode.h
  ./Front/Mono/CstMNodePrivate.h
  ./Front/Mono/CstMComponent.c
  ./Front/Mono/CstMComponent.h
  ./Front/Mono/CstMText.c
  ./Front/Mono/CstMText.h
  ./Front/Mono/CstMWidget.c
  ./Front/Mono/CstMWidget.h
)

set(C_API_SRC
  ./Front/C/CstCType.h

  ./Front/C/CstCNode.c
  ./Front/C/CstCNode.h
  ./Front/C/CstCWidget.c
  ./Front/C/CstCWidget.h
  ./Front/C/CstCComNode.c
  ./Front/C/CstCComNode.h
  ./Front/C/CstCLFlex.c
  ./Front/C/CstCLFlex.h
  ./Front/C/CstCLFlow.c
  ./Front/C/CstCLFlow.h
  ./Front/C/CstCLBox.c
  ./Front/C/CstCLBox.h
  ./Front/C/CstCLDiv.c
  ./Front/C/CstCLDiv.h
  ./Front/C/CstCLBody.c
  ./Front/C/CstCLBody.h
  ./Front/C/CstCLGrid.c
  ./Front/C/CstCLGrid.h
  ./Front/C/CstCText.c
  ./Front/C/CstCText.h
  ./Front/C/CstCComponent.c
  ./Front/C/CstCComponent.h
)

set(SRC
  ./BuildIn/Components/CstLabel.h
  ./BuildIn/Components/CstLabel.c
  ./BuildIn/Components/CstLabel.cst

  ./Driver/CstTypes.h
  ./Driver/CstMacros.h
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
  ./Driver/CstBoxLayer.c
  ./Driver/CstBoxLayer.h
  ./Driver/CstAbsLayer.c
  ./Driver/CstAbsLayer.h

  ./Driver/CstManager.c
  ./Driver/CstManager.h
  ./Driver/CstModule.c
  ./Driver/CstModule.h

  ./Driver/Css/CstCssValue.c
  ./Driver/Css/CstCssValue.h
  ./Driver/Css/CstCssEnv.c
  ./Driver/Css/CstCssEnv.h
  ./Driver/Css/CstCss.c
  ./Driver/Css/CstCss.h
  ./Driver/Css/CstCssPrivate.h

  ./Front/Common/CstNodePrivate.h
  ./Front/Common/CstNode.h
  ./Front/Common/CstNode.c
  ./Front/Common/CstComNode.h
  ./Front/Common/CstComNode.c
  ./Front/Common/CstNodeMap.h
  ./Front/Common/CstNodeMap.c
  ./Front/Common/CstPropMap.h
  ./Front/Common/CstPropMap.c
  ./Front/Common/CstPropPair.h
  ./Front/Common/CstPropPair.c
  ./Front/Common/CstNodeMapCore.h
  ./Front/Common/CstComponentPrivate.h
  ./Front/Common/CstComponent.h
  ./Front/Common/CstComponent.c
  ./Front/Common/CstTextPrivate.h
  ./Front/Common/CstText.h
  ./Front/Common/CstText.c
  ./Front/Common/CstWidgetPrivate.h
  ./Front/Common/CstWidget.h
  ./Front/Common/CstWidget.c
  ./Front/Common/CstLBodyPrivate.h
  ./Front/Common/CstLBody.h
  ./Front/Common/CstLBody.c
  ./Front/Common/CstLBoxPrivate.h
  ./Front/Common/CstLBox.h
  ./Front/Common/CstLBox.c
  ./Front/Common/CstLDiv.h
  ./Front/Common/CstLDiv.c
  ./Front/Common/CstLFlex.h
  ./Front/Common/CstLFlex.c
  ./Front/Common/CstLGridPrivate.h
  ./Front/Common/CstLGrid.h
  ./Front/Common/CstLGrid.c
  ./Front/CstAnime.c
  ./Front/CstAnime.h
  ./Front/CstFrontCore.h

  ./Parser/CstCli.l
  ./Parser/CstCli.y
  ./Parser/CstCli.yy.h
  ./Parser/CstCli.yy.c
  ./Parser/lex.yy.c

  ./Parser/Ast.c
  ./Parser/Ast.h
  ./Parser/AstJNode.c
  ./Parser/AstJNode.h
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
    LIST(APPEND SRC ${MONO_API_SRC})
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
