set(INC
  ../
)

set(EXTERNAL_LIBS
  "System"
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
  "fontconfig"
  "tinyexpr"
  "ffmpeg"
  "png"
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
  ./ThirdParty/FrLibav.c
  ./ThirdParty/FrLibav.h

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

  ./Media/FrAudioDecoder.c
  ./Media/FrAudioDecoder.h
  ./Media/FrAudioFrame.c
  ./Media/FrAudioFrame.h
  ./Media/FrDecoder.c
  ./Media/FrDecoder.h
  ./Media/FrFrameDecoder.c
  ./Media/FrFrameDecoder.h
  ./Media/FrGlRender.c
  ./Media/FrGlRender.h
  ./Media/FrIDecoder.c
  ./Media/FrIDecoder.h
  ./Media/FrImageScale.c
  ./Media/FrImageScale.h
  ./Media/FrIMedia.c
  ./Media/FrIMedia.h
  ./Media/FrIMediaRender.c
  ./Media/FrIMediaRender.h
  ./Media/FrIStream.c
  ./Media/FrIStream.h
  ./Media/FrMedia.c
  ./Media/FrMedia.h
  ./Media/FrMediaTask.c
  ./Media/FrMediaTask.h
  ./Media/FrMediaDecoder.c
  ./Media/FrMediaDecoder.h
  ./Media/FrMediaFile.c
  ./Media/FrMediaFile.h
  ./Media/FrMediaFilter.c
  ./Media/FrMediaFilter.h
  ./Media/FrMediaFrame.c
  ./Media/FrMediaFrame.h
  ./Media/FrMediaPacket.c
  ./Media/FrMediaPacket.h
  ./Media/FrMediaPlayer.c
  ./Media/FrMediaPlayer.h
  ./Media/FrPlayer.c
  ./Media/FrPlayer.h
  ./Media/FrMediaStream.c
  ./Media/FrMediaStream.h
  ./Media/FrPacket.c
  ./Media/FrPacket.h
  ./Media/FrPacketDecoder.c
  ./Media/FrPacketDecoder.h
  ./Media/FrPipeline.c
  ./Media/FrPipeline.h
  ./Media/FrMediaPipeline.c
  ./Media/FrMediaPipeline.h
  ./Media/FrStream.c
  ./Media/FrStream.h
  ./Media/FrVideoDecoder.c
  ./Media/FrVideoDecoder.h
  ./Media/FrVideoFrame.c
  ./Media/FrVideoFrame.h
  ./Media/FrMediaCommon.c
  ./Media/FrMediaCommon.h
  ./Media/FrMediaTypes.h
  ./Media/FrMediaCore.h
  ./Media/FrMediaCore.c

  ./Device/FrIDevice.c
  ./Device/FrIDevice.h
  ./Device/FrDevice.c
  ./Device/FrDevice.h
  ./Device/FrDisplay.c
  ./Device/FrDisplay.h
  ./Device/FrImageDevice.c
  ./Device/FrImageDevice.h
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
  ./Graph/FrIDraw.c
  ./Graph/FrIDraw.h
  ./Graph/FrDrawContext.c
  ./Graph/FrDrawContext.h
  ./Graph/FrCairoDrawContext.c
  ./Graph/FrCairoDrawContext.h
  ./Graph/FrImage.c
  ./Graph/FrImage.h

  ./Graph/FrSurface.c
  ./Graph/FrSurface.h
  ./Graph/FrContext.c
  ./Graph/FrContext.h
  # ./Graph/FrText.c
  # ./Graph/FrText.h

  ./FrTypes.h
  ./FrCommon.h
  ./FrCommon.c
  ./FrCore.c
  ./FrCore.h
  ./FrConfig.h
  ./FrConfig.h.in
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

add_library(Framework "${SRC}")
add_deps_N(Framework
  "${INC}"
  "${INNER_LIBS}"
  ""
  "${EXTERNAL_LIBS}"
  ""
)
configure_file(${CMAKE_CURRENT_LIST_DIR}/FrConfig.h.in
  ${CMAKE_CURRENT_LIST_DIR}/FrConfig.h)
set_property(TARGET Framework PROPERTY FOLDER CstProject)
