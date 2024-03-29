set(INC
  ../

  ${GLIB_INCLUDE_DIRS}
  ${PANGO_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${VLD_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
  ${GLFW3_INCLUDE_DIRS}
  ${TINYEXPR_INCLUDE_DIRS}
  ${PANGOFC_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIRS}
)

set(INC_SYS
)

set(UNIX_SRC
  ./Platform/Unix/FrDisplayX11.c
  ./Platform/Unix/FrWindowX11.c
)

set(WIN32_SRC
  ./Platform/Win32/FrDisplayWin32.c
  ./Platform/Win32/FrWindowWin32.c
)

set(COCOA_SRC
)

set(MONO_SRC
  # ./FrMono.h
  # ./FrMono.c
)

set(SRC
  ./ThirdParty/FrGlfw.h
  ./ThirdParty/FrGlfw.c
  ./ThirdParty/FrGlib.h
  ./ThirdParty/FrGlib.c
  ./ThirdParty/FrCairo.h
  ./ThirdParty/FrCairo.c
  ./ThirdParty/FrFont.c
  ./ThirdParty/FrFont.h
  ./ThirdParty/FrExpr.h
  ./ThirdParty/FrExpr.c

  ./Event/FrEventCore.h
  ./Event/Base/FrEvent.c
  ./Event/Base/FrEvent.h
  ./Event/Base/FrEventAny.c
  ./Event/Base/FrEventAny.h
  ./Event/Base/FrEventKey.h
  ./Event/Base/FrEventKey.c
  ./Event/Base/FrEventMouseKey.h
  ./Event/Base/FrEventMouseKey.c
  ./Event/Base/FrEventRefresh.h
  ./Event/Base/FrEventRefresh.c
  ./Event/Base/FrEventMapping.h
  ./Event/Base/FrEventCursorMove.c
  ./Event/Base/FrEventCursorMove.h

  ./Event/Action/FrAWatch.c
  ./Event/Action/FrAWatch.h
  ./Event/Action/FrAWatchBuilder.c
  ./Event/Action/FrAWatchBuilder.h
  ./Event/Action/FrAWatchKey.c
  ./Event/Action/FrAWatchKey.h
  ./Event/Action/FrAWatchAny.c
  ./Event/Action/FrAWatchAny.h
  ./Event/Action/FrAWatchRefresh.c
  ./Event/Action/FrAWatchRefresh.h
  ./Event/Action/FrAWatchKeyPress.c
  ./Event/Action/FrAWatchKeyPress.h
  ./Event/Action/FrAWatchMousePress.c
  ./Event/Action/FrAWatchMousePress.h
  ./Event/Action/FrAWatchMouseRelease.c
  ./Event/Action/FrAWatchMouseRelease.h
  ./Event/Action/FrAWatchCursorMove.c
  ./Event/Action/FrAWatchCursorMove.h

  ./Event/Action/FrAction.c
  ./Event/Action/FrAction.h
  ./Event/Action/FrAKey.c
  ./Event/Action/FrAKey.h
  ./Event/Action/FrAMouseKey.c
  ./Event/Action/FrAMouseKey.h
  ./Event/Action/FrACursorMove.c
  ./Event/Action/FrACursorMove.h
  ./Event/FrEvents.c
  ./Event/FrEvents.h

  ./Device/FrIDevice.c
  ./Device/FrIDevice.h
  ./Device/FrDisplay.c
  ./Device/FrDisplay.h
  ./Device/FrWindow.c
  ./Device/FrWindow.h

  ./DataType/FrFunc.c
  ./DataType/FrFunc.h
  ./DataType/FrMain.c
  ./DataType/FrMain.h
  ./DataType/FrSource.c
  ./DataType/FrSource.h
  ./DataType/FrApplication.c
  ./DataType/FrApplication.h
  ./DataType/FrWorker.c
  ./DataType/FrWorker.h
  ./DataType/FrPair.h
  ./DataType/FrPair.c
  ./DataType/FrNode.h
  ./DataType/FrNode.c
  ./DataType/FrEnv.c
  ./DataType/FrEnv.h

  ./Graph/FrGraph.c
  ./Graph/FrGraph.h
  ./Graph/FrDraw.c
  ./Graph/FrDraw.h
  ./Graph/FrIDraw.c
  ./Graph/FrIDraw.h
  ./Graph/FrDrawContext.c
  ./Graph/FrDrawContext.h
  ./Graph/FrSurface.c
  ./Graph/FrSurface.h
  ./Graph/FrContext.c
  ./Graph/FrContext.h
  ./Graph/FrCairoDraw.c
  ./Graph/FrCairoDraw.h
  # ./Graph/FrText.c
  # ./Graph/FrText.h

  ./FrTypes.h
  ./FrCommon.h
  ./FrCommon.c
  ./FrCore.c
  ./FrCore.h
)

if(USE_MONO)
  LIST(APPEND SRC ${MONO_SRC})
endif()

if(WIN32)
  LIST(APPEND SRC ${WIN32_SRC})
elseif(UNIX)
  LIST(APPEND SRC ${UNIX_SRC})
elseif(APPLE)
  LIST(APPEND SRC ${COCOA_SRC})
endif()

add_dep_libs(FRFramework "${SRC}" "${INC}" "${INC_SYS}")

set_property(TARGET FRFramework PROPERTY FOLDER CstProject)

add_dependencies(FRFramework
  System
)

target_link_libraries(FRFramework
  System
  ${OPENSSL_LIBRARIES}
  ${PANGOFC_LIBRARIES}
  ${GLIB_LIBRARIES}
  ${MONO_LIBRARIES}
  ${PIXMAN_LIBRARIES}
  ${X11_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${PANGOCAIRO_LIBRARIES}
  ${PANGO_LIBRARIES}
  ${HARFBUZZ_LIBRARIES}
  ${VLD_LIBRARIES}
  ${TINYEXPR_LIBRARIES}
  ${GLAD_LIBRARIES}
  ${GLFW3_LIBRARIES}
  ${X11_LIBRARIES}
  ${LLVM_LIBRARIES}
  ${GL_LIBRARIES}
  ${FONTCONFIG_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${VULKAN_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${VKVG_LIBRARIES}
  ${SYSTEM_LIBRARIES}
)
