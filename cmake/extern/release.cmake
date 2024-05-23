include(${CMAKE_CURRENT_LIST_DIR}/env.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../macros/logger.cmake)

function(release_copy_files
    target_name
    target_path
    target_bin)

  if(NOT EXISTS "${LIBDIR}")
    message(FATAL_ERROR "Windows requires pre-compiled libs at: '${LIBDIR}'")
  endif()

  if(${CMAKE_BUILD_TYPE} STREQUAL "Release") 
    log("Copy file to: ${LIBDIR}/${target_name}/lib")
  else()
    return()
  endif()

  file(COPY ${target_path}
    DESTINATION ${LIBDIR}/${target_name}/include
    FILES_MATCHING
    PATTERN "*.h")

  if(WIN32)
    file(COPY ${target_bin}/${CMAKE_BUILD_TYPE}/${target_name}.lib
      DESTINATION ${LIBDIR}/${target_name}/lib
      FILES_MATCHING
      PATTERN "*.lib")
  elseif(UNIX)
    file(COPY ${target_bin}/lib${target_name}.a
      DESTINATION ${LIBDIR}/${target_name}/lib
      FILES_MATCHING
      PATTERN "*.a")
  endif()
endfunction()
release_copy_files(
  ${target_name}
  ${target_path}
  ${target_bin})
