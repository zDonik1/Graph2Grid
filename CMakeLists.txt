cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# project info
project(
  Graph2Grid
  VERSION 0.1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
include(3rd/PackageProject/CMakeLists.txt)

# ---- Add source files ----

set(HEADERS
    include/graph2grid/system.h
    include/graph2grid/tree_parse/tree_node_system.h
    include/graph2grid/tree_parse/tree_node_component.h
    include/graph2grid/tree_parse/range.h
    include/graph2grid/tree_parse/near_component.h
    include/graph2grid/tree_parse/tree_node_system_stack.h
    include/graph2grid/tree_parse/ggdl_lexer.h
)

set(SOURCES
    source/system.cpp
    source/tree_parse/tree_node_system.cpp
    source/tree_parse/tree_node_component.cpp
    source/tree_parse/near_component.cpp
    source/tree_parse/tree_node_system_stack.cpp
    source/tree_parse/ggdl_lexer.cpp
)

# ---- Create library ----

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

# setting c++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<BOOL:${MSVC}>:/permissive->")

# adding include directories for build and install
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rd/spimpl>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/spimpl>
)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

# listing 3rd party library includes
set(INSTALL_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/include/
  ${PROJECT_SOURCE_DIR}/3rd/spimpl
)

# doing installs
packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIRS ${INSTALL_INCLUDE_DIRS}
  INCLUDE_DESTINATION include
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  DEPENDENCIES ""
)

# packaging
set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
include(CPack)
