include(${CMAKE_CURRENT_LIST_DIR}/options.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../macros/logger.cmake)

function(release_copy_files
    target_name
    target_path)

  file(COPY ./
    DESTINATION ${LIBDIR}/${target_name}/include/${target_name}
    FILES_MATCHING
    PATTERN "*.h")

  file(COPY ${CMAKE_CURRENT_BINARY_DIR}/Release/${target_name}.lib
    DESTINATION ${LIBDIR}/${target_name}/lib
    FILES_MATCHING
    PATTERN "*.lib")
endfunction()
release_copy_files(${target_name} ${target_path})
