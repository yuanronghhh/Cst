include(${CMAKE_CURRENT_LIST_DIR}/options.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../macros/logger.cmake)

function(release_copy_files
    target_name
    target_path
    target_bin)

  file(COPY ${target_path}
    DESTINATION ${LIBDIR}/${target_name}/include
    FILES_MATCHING
    PATTERN "*.h")

  if(WIN32)
    file(COPY ${target_bin}/${target_name}
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
