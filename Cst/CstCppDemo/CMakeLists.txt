# TEST
include_dep_dirs(
  ./
  ../system
  ../unity
  ../vkvg/include
  ${VLD_INCLUDE_DIRS}
  ${GLIB_INCLUDE_DIRS}
  ${CGLM_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
  ${GLFW3_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIRS}
  ${PIXMAN_INCLUDE_DIR}
  ${UNITY_INCLUDE_DIRS}
  ${VULKAN_INCLUDE_DIRS}
  ${VKVG_INCLUDE_DIRS}
  ${STB_INCLUDE_DIRS}
)

set(TEST_SRC
  ./Config.h
  ./Config.h.in

  ./Demo.cpp
  ./TutorialVulkan.cpp
  ./TutorialVulkan.hpp
)

set(LANG cpp)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/std:c++17>")
add_executable(CstCppDemo "${TEST_SRC}" "${TEST_INC}" "")
set_property(TARGET CstCppDemo PROPERTY FOLDER CstProject)

configure_file(${CMAKE_CURRENT_LIST_DIR}/Config.h.in
               ${CMAKE_CURRENT_LIST_DIR}/Config.h)


target_link_libraries(CstCppDemo
  ${VLD_LIBRARIES}
  ${UNITY_LIBRARIES}
  ${GLIB_LIBRARIES}
  ${VULKAN_LIBRARIES}
  ${GLFW3_LIBRARIES}
  ${VKVG_LIBRARIES}
  ${CGLM_LIBRARIES}
)
