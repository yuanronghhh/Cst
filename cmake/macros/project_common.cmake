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
        name
    )

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
        includes_sys
    )

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
