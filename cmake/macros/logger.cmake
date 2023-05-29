function(log_list_all_variables)
  get_cmake_property(_variableNames VARIABLES)
  list (SORT _variableNames)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endfunction()

macro(log
    message
  )
  message(STATUS "[${CMAKE_CURRENT_LIST_DIR}] ${message}")
endmacro()
