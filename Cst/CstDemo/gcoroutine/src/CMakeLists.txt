include_dep_dirs(INC
  ./
  ${GLIB_INCLUDE_DIRS}
)

set(SRC_WIN32
  ./gcoroutine-winfiber.c
  # ./ucontext.c
  # ./ucontext.h
  # ./gcoroutine-ucontext.c
)

set(SRC_UNIX
  ./gcoroutine-gthread.c
  ./valgrind.h
)

set(SRC
  ./config.h.in
  ./gcoroutine.pc
  ./gcoroutine.pc.in
  ./gcoroutine-version.h.in

  ./config.h
  ./gcoroutine-version-macros.h
  ./gcoroutine-version.h

  ./gcoroutine.c
  ./gcoroutine.h

  ./gcoroutine-macros.h
  ./gcoroutineprivate.h
)

set(GTK_DOC_REQ
  ${CMAKE_CURRENT_LIST_DIR}/gcoroutine-docs.xml
  ${CMAKE_CURRENT_LIST_DIR}/gcoroutine-overrides.txt
  ${CMAKE_CURRENT_LIST_DIR}/gcoroutine-sections.txt
  ${CMAKE_CURRENT_LIST_DIR}/gcoroutine.types
)
LIST(APPEND SRC ${GTK_DOC_REQ})

if(UNIX)
  LIST(APPEND SRC ${SRC_UNIX})
elseif(WIN32)
  LIST(APPEND SRC ${SRC_WIN32})
endif()

add_library(gcoroutine "${SRC}")
set_property(TARGET gcoroutine PROPERTY FOLDER CstProject)
set_target_properties(gcoroutine
  PROPERTIES VcpkgEnableManifest true
)

target_link_libraries(gcoroutine
  ${GLIB_LIBRARIES}
)

# if(UNIX)
#   set(GTK_DOC_MODULE gcoroutine)
#   set(GTK_DOC_OUTPUT "${CMAKE_SOURCE_DIR}/doc/html")
# 
#   add_custom_command(
#     OUTPUT "doc-req"
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GTK_DOC_REQ} ${CMAKE_CURRENT_BINARY_DIR}
#   )
# 
#   gtk_doc_add_module(gcoroutine
#     SOURCE ${CMAKE_CURRENT_LIST_DIR}
#     LIBRARIES gcoroutine
#     LIBRARY_DIRS ${CMAKE_CURRENT_LIST_DIR}
#     MKDBOPTS "--output-format=xml" "--name-space=gcoroutine"
#     SCANOPTS "--rebuild-types" "--deprecated-guards=\"GCOROUTINE_DISABLE_DEPRECATED\"" "--ignore-decorators=\"GCOROUTINE_FUNC\""
#     IGNOREHEADERS "config.h valgrind.h gcoroutineprivate.h"
#   )
# 
#   add_custom_target(build-req ALL DEPENDS doc-req)
#   add_custom_target(documentation ALL DEPENDS doc-gcoroutine)
# endif()
