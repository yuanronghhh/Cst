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
)

set(INC_SYS
)

set(UNIX_SRC
  ./Platform/Unix/FRDisplayX11.c
  ./Platform/Unix/FRWindowX11.c
)

set(WIN32_SRC
  ./Platform/Win32/FRDisplayWin32.c
  ./Platform/Win32/FRWindowWin32.c
)

set(COCOA_SRC
)

set(MONO_SRC
  # ./FRMono.h
  # ./FRMono.c
)

set(SRC
  ./ThirdParty/FRGlfw.h
  ./ThirdParty/FRGlfw.c
  ./ThirdParty/FRGlib.h
  ./ThirdParty/FRGlib.c
  ./ThirdParty/FRCairo.h
  ./ThirdParty/FRCairo.c
  ./ThirdParty/FRFont.c
  ./ThirdParty/FRFont.h
  ./ThirdParty/FRExpr.h
  ./ThirdParty/FRExpr.c

  ./Event/FREventCore.h
  ./Event/Base/FREvent.c
  ./Event/Base/FREvent.h
  ./Event/Base/FREventAny.c
  ./Event/Base/FREventAny.h
  ./Event/Base/FREventKey.h
  ./Event/Base/FREventKey.c
  ./Event/Base/FREventMouseKey.h
  ./Event/Base/FREventMouseKey.c
  ./Event/Base/FREventRefresh.h
  ./Event/Base/FREventRefresh.c
  ./Event/Base/FREventMapping.h
  ./Event/Base/FREventCursorMove.c
  ./Event/Base/FREventCursorMove.h

  ./Event/Action/FRAWatch.c
  ./Event/Action/FRAWatch.h
  ./Event/Action/FRAWatchKey.c
  ./Event/Action/FRAWatchKey.h
  ./Event/Action/FRAWatchAny.c
  ./Event/Action/FRAWatchAny.h
  ./Event/Action/FRAWatchRefresh.c
  ./Event/Action/FRAWatchRefresh.h
  ./Event/Action/FRAWatchKeyPress.c
  ./Event/Action/FRAWatchKeyPress.h
  ./Event/Action/FRAWatchMousePress.c
  ./Event/Action/FRAWatchMousePress.h
  ./Event/Action/FRAWatchMouseRelease.c
  ./Event/Action/FRAWatchMouseRelease.h

  ./Event/Action/FRAction.c
  ./Event/Action/FRAction.h
  ./Event/Action/FRAKey.c
  ./Event/Action/FRAKey.h
  ./Event/Action/FRAMouseKey.c
  ./Event/Action/FRAMouseKey.h
  ./Event/Action/FRACursorMove.c
  ./Event/Action/FRACursorMove.h
  ./Event/FREvents.c
  ./Event/FREvents.h

  ./Device/FRDisplay.c
  ./Device/FRDisplay.h
  ./Device/FRDisplayPrivate.h
  ./Device/FRWindow.c
  ./Device/FRWindow.h
  ./Device/FRWindowPrivate.h

  ./DataType/FRFunc.c
  ./DataType/FRFunc.h
  ./DataType/FRMain.c
  ./DataType/FRMain.h
  ./DataType/FRSource.c
  ./DataType/FRSource.h
  ./DataType/FRApplication.c
  ./DataType/FRApplication.h
  ./DataType/FRWorker.c
  ./DataType/FRWorker.h
  ./DataType/FRPQueue.h
  ./DataType/FRPQueue.c
  ./DataType/FREnv.c
  ./DataType/FREnv.h

  ./Graph/FRGraph.c
  ./Graph/FRGraph.h
  ./Graph/FRDraw.c
  ./Graph/FRDraw.h
  # ./Graph/FRText.c
  # ./Graph/FRText.h
  ./Graph/FRCanvasPrivate.h
  ./Graph/FRCanvas.h
  ./Graph/FRCanvas.c

  ./FRTypes.h
  ./FRCommon.h
  ./FRCommon.c
  ./FRCore.h
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
  ${PANGOFC_LIBRARIES}
  ${GLIB_LIBRARIES}
  ${MONO_LIBRARIES}
  ${PIXMAN_LIBRARIES}
  ${X11_LIBRARIES}
  ${CAIRO_LIBRARIES}
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
