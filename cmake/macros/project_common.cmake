##
# most code from 
# https://github.com/blender/blender/blob/master/build_files/cmake/macros.cmake
##
function(list_assert_duplicates
    list_id
)
  # message(STATUS "list data: ${list_id}")

  list(REMOVE_DUPLICATES list_id)
endfunction()

macro(add_dep_for_libray_N name
    external_libs
)
  string(TOUPPER ${name} _UPPER_NAME)

  foreach(_LIB ${external_libs})
    string(TOUPPER ${_LIB} _UPPER_LIB)
    list(APPEND ${_UPPER_NAME}_INCLUDE_DIR "${${_UPPER_LIB}_INCLUDE_DIRS}")
    list(APPEND ${_UPPER_NAME}_LIBRARY "${${_UPPER_LIB}_LIBRARIES}")
    list(APPEND ${_UPPER_NAME}_FILES "${${_UPPER_LIB}_FILES}")
  endforeach()
endmacro()

function(add_deps_options_N
    app_name
    INNER_INCS
    INNER_LIBS
    EXTERNAL_INCS
    EXTERNAL_LIBS
    EXTERNAL_OPTIONS
  )
  set(_INCS "")
  set(_LIBS "")
  set(_FILES "")

  foreach(_LIB ${EXTERNAL_LIBS})
    string(TOUPPER ${_LIB} _UPPER_LIB)
    set(_DIR_VAR ${${_UPPER_LIB}_INCLUDE_DIRS})

    list(APPEND _INCS "${_DIR_VAR}")
    list(APPEND _LIBS "${${_UPPER_LIB}_LIBRARIES}")
    list(APPEND _FILES "${${_UPPER_LIB}_FILES}")
  endforeach()

  foreach(_LIB ${INNER_INCS})
    get_filename_component(_ABS_INC ${_LIB} ABSOLUTE)

    list(APPEND _INCS "${_ABS_INC}")
  endforeach()

  foreach(_LIB ${INNER_LIBS})
    list(APPEND _LIBS "${_LIB}")
  endforeach()

  LIST(APPEND _LIBS ${EXTERNAL_OPTIONS})
  list(REMOVE_DUPLICATES _INCS)
  list(REMOVE_DUPLICATES _LIBS)
  list(REMOVE_DUPLICATES _FILES)

  include_directories(${_INCS})
  IF ("${INNER_LIBS}" STREQUAL "")
  else()
  endif()
  target_link_libraries(${app_name} ${_LIBS})
  target_copy_files(${app_name} ${_FILES})
endfunction()

function(add_deps_N
    app_name
    INNER_INCS
    INNER_LIBS
    EXTERNAL_INCS
    EXTERNAL_LIBS)

  add_deps_options_N(
    "${app_name}"
    "${INC}"
    "${INNER_LIBS}"
    "${EXTERNAL_INCS}"
    "${EXTERNAL_LIBS}"
    "")
endfunction()

function(include_dep_dirs
    includes
)
  set(_ALL_INCS "")
  foreach(_INC ${ARGV})
    get_filename_component(_ABS_INC ${_INC} ABSOLUTE)
    list(APPEND _ALL_INCS ${_ABS_INC})
  endforeach()
  include_directories(${_ALL_INCS})
endfunction()

function(include_dep_dirs_sys
    includes
    )

  set(_ALL_INCS "")
  foreach(_INC ${ARGV})
    get_filename_component(_ABS_INC ${_INC} ABSOLUTE)
    list(APPEND _ALL_INCS ${_ABS_INC})
  endforeach()
  include_directories(SYSTEM ${_ALL_INCS})
endfunction()

function(code_source_group
    sources
    )

  # source_group("Source Files" FILES CMakeLists.txt)

  foreach(_SRC ${sources})
    get_filename_component(_SRC_EXT ${_SRC} EXT)
    if((${_SRC_EXT} MATCHES ".h") OR
      (${_SRC_EXT} MATCHES ".hpp") OR
      (${_SRC_EXT} MATCHES ".hh"))

      set(GROUP_ID "Header Files")
    elseif((${_SRC_EXT} MATCHES ".c") OR
      (${_SRC_EXT} MATCHES ".cpp") OR
      (${_SRC_EXT} MATCHES ".cc"))
      set(GROUP_ID "Source Files")
    else()
    endif()
    source_group("${GROUP_ID}" FILES ${_SRC})
  endforeach()
endfunction()

function(source_group_to
    group_id
    sources
    )

  foreach(_SRC ${sources})
    get_filename_component(_ABS_SRC ${_SRC} ABSOLUTE)
    source_group("${group_id}" FILES "${_ABS_SRC}")
  endforeach()
endfunction()


function(add_dep_libs__impl
    name
    sources
    includes
    includes_sys
    )

  # message(STATUS "Configuring library ${name}")

  include_dep_dirs("${includes}")
  include_dep_dirs_sys("${includes_sys}")

  add_library(${name} STATIC ${sources})

  code_source_group("${sources}")

  list_assert_duplicates("${sources}")
  list_assert_duplicates("${includes}")
endfunction()

macro(add_cc_flags_custom_test
    name)

  string(TOUPPER ${name} _name_upper)
  message(STATUS "${name}")
  if(DEFINED CMAKE_C_FLAGS_${_name_upper})
    message(STATUS "Using custom CFLAGS: CMAKE_C_FLAGS_${_name_upper} in \"${CMAKE_CURRENT_SOURCE_DIR}\"")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${_name_upper}}" ${ARGV1})
  endif()
  if(DEFINED CMAKE_CXX_FLAGS_${_name_upper})
    message(STATUS "Using custom CXXFLAGS: CMAKE_CXX_FLAGS_${_name_upper} in \"${CMAKE_CURRENT_SOURCE_DIR}\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${_name_upper}}" ${ARGV1})
  endif()
  unset(_name_upper)
endmacro()

function(add_dep_libs
    name
    sources
    includes
    includes_sys)

  add_cc_flags_custom_test(${name} PARENT_SCOPE)

  add_dep_libs__impl(${name} "${sources}" "${includes}" "${includes_sys}")

  set_property(GLOBAL APPEND PROPERTY LINK_LIBS ${name})
endfunction()

macro(define_include_var
    uname
    value
)
  set(_ALL_INCS "")
  foreach(_INC ${ARGV})
    get_filename_component(_ABS_INC ${_INC} ABSOLUTE)
    list(APPEND _ALL_INCS ${_ABS_INC})
  endforeach()
  define_var_cached(${uname} ${value})
endmacro()

function(target_copy_release_files
    name
)
  add_custom_command(TARGET ${name} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -Dtarget_name=${name}
    -Dtarget_path=${CMAKE_CURRENT_SOURCE_DIR}
    -Dtarget_bin=${CMAKE_CURRENT_BINARY_DIR}
    -DLIBDIR=${LIBDIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -P ${CMAKE_MODULE_PATH}/../extern/release.cmake
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endfunction()

function(target_copy_files)
  if(WIN32)
    # Parse the arguments
    cmake_parse_arguments(ARGS "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(TSOURCE "")
    set(TNAME "")
    foreach(FName ${ARGN})
      if(NOT IS_ABSOLUTE ${FName})
        set(TNAME ${FName})
      else()
        LIST(APPEND TSOURCE ${FName})
      endif()
    endforeach()

    add_custom_command(
      TARGET ${TNAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TSOURCE} $<TARGET_FILE_DIR:${TNAME}>)
  endif()
endfunction()

macro(found_module
    components
    prefix_name
    suffix_name)

  STRING(TOUPPER ${prefix_name} PREFIX_NAME)

  FOREACH(COMPONENT ${components})
    STRING(TOUPPER ${COMPONENT} COMPONENT_NAME)

    FIND_LIBRARY(${PREFIX_NAME}_${COMPONENT_NAME}_LIBRARY
      NAMES "${COMPONENT}${suffix}"
      HINTS ${search_dirs}
      PATH_SUFFIXES lib64 lib ${COMPONENT}/lib)

    if(WIN32)
      FIND_FILE(${PREFIX_NAME}_${COMPONENT_NAME}_FILE
        NAMES "${COMPONENT}${suffix_name}"
        HINTS ${search_dirs}
        PATH_SUFFIXES ${prefix_name}/bin)

      LIST(APPEND ${PREFIX_NAME}_FILES "${PREFIX_NAME}_${COMPONENT_NAME}_FILE")
    endif()

    LIST(APPEND ${PREFIX_NAME}_LIBRARY "${PREFIX_NAME}_${COMPONENT_NAME}_LIBRARY")
  ENDFOREACH()
endmacro()

