cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# project info
project(tests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# ---- Variables ----

# setting testing target
set(TEST_TARGET Graph2Grid)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME doctest
  GITHUB_REPOSITORY onqtam/doctest
  GIT_TAG 2.3.7
)

# finding testing target
if(TEST_INSTALLED_VERSION)
  find_package(${TEST_TARGET} REQUIRED)
else()
  CPMAddPackage(NAME ${TEST_TARGET} SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

CPMAddPackage(
  NAME Format.cmake
  GITHUB_REPOSITORY TheLartians/Format.cmake
  VERSION 1.6
  OPTIONS # enable cmake formatting
          "FORMAT_CHECK_CMAKE ON"
)

# ---- Create binary ----

# listing sources (CHANGE)
set(SOURCES
  source/main.cpp
  source/test.cpp
  source/tree_parse_test.cpp
)

# making the exectuable
add_executable(${PROJECT_NAME} ${SOURCES})

# linking external libs
target_link_libraries(${PROJECT_NAME} doctest Graph2Grid::Graph2Grid)

# setting cpp standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${TEST_TARGET} PUBLIC -Wall -pedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(${TEST_TARGET} PUBLIC /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Copying additonal files ----

file(COPY city_config.ggdl
     DESTINATION ${PROJECT_BINARY_DIR})

# ---- Add Tests ----

enable_testing()

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(${TEST_TARGET} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${TEST_TARGET} PUBLIC -fprofile-arcs -ftest-coverage)
endif()
