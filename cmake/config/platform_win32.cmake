set(OS_ARCHITECH "Win64")
set(LIBDIR ${CMAKE_SOURCE_DIR}/../lib/${OS_ARCHITECH}_vc14)

if(NOT EXISTS "${LIBDIR}/")
  message(FATAL_ERROR "Windows requires pre-compiled libs at: '${LIBDIR}'")
endif()

# find_package(mono REQUIRED)
find_package(fontconfig REQUIRED)
find_package(freetype REQUIRED)
find_package(pixman REQUIRED)
find_package(pthread REQUIRED)
find_package(tinyexpr REQUIRED)
find_package(fribidi REQUIRED)
find_package(unity REQUIRED)
find_package(gcoroutine REQUIRED)
find_package(glib REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(stb REQUIRED)
find_package(pthread REQUIRED)
find_package(vld REQUIRED)
find_package(harfbuzz REQUIRED)
find_package(pango REQUIRED)
find_package(png REQUIRED)
find_package(zlib REQUIRED)
find_package(cairo REQUIRED)
find_package(gtk REQUIRED)

set(SDL2_MIXER_DIR ${SDL2_DIR})
set(SDL2_MIXER_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
set(SDL2_MIXER_DLL ${SDL2_MIXER_DLL}/lib/SDL2_mixer.dll)
set(SDL2_MIXER_LIBRARIES ${SDL2_DIR}/lib/SDL2_mixer.lib)

set(MPG123_LIBRARIES "libmpg123.lib")

set(DWM_INCLUDE_DIRS "c:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um")
set(DWM_LIBRARIES dwmapi.lib)

set(GL_INCLUDE_DIRS "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um/gl")
set(GL_LIBRARIES opengl32.lib)

set(ADDTIONAL_LIBRARIES kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib)
set(CRT_LIBRAREIS
  "libucrt.lib"
  "ucrt.lib"
  "ucrtd.lib"
  "libucrtd.lib")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions("/MT")
add_definitions("/MP")
add_definitions("/WX")

set(DBG_LIBRAREIS "Dbghelp.lib")

# set(GLUT_INCLUDE_DIRS ${LIBDIR}/opengl/include)
# set(GLUT_LIBRARIES ${OPENGL_DIR}/lib/freeglut_static.lib)
