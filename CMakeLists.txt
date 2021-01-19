project(BOO)
#**************************************************************************************************
# General cMake settings
#**************************************************************************************************
cmake_minimum_required(VERSION 3.5)

#**************************************************************************************************
# Find Package
#**************************************************************************************************
find_library(catch2 REQUIRED)
find_path(CATCH_INCLUDE_DIR catch2/catch.hpp)
include_directories(${CATCH_INCLUDE_DIR})


#**************************************************************************************************
# Include
#**************************************************************************************************
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


#**************************************************************************************************
# Set variable
#**************************************************************************************************
set(SOURCES
  engine.cpp
)

#**************************************************************************************************
# Set compiler
#**************************************************************************************************
add_compile_options(-std=c++17 -Wall) # for all targets in current directory


#**************************************************************************************************
# Linker
#**************************************************************************************************
LINK_DIRECTORIES(
)


#**************************************************************************************************
# Make configuration
#**************************************************************************************************
add_executable(boo ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_compile_options(boo PRIVATE -O3)
target_link_libraries(boo -pthread -lstdc++fs)

#**************************************************************************************************
# Test configuration
#**************************************************************************************************
option(BUILD_UNIT_TESTS "Build the unit tests" ON)
if(BUILD_UNIT_TESTS)
  enable_testing()
  SET(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-main.cpp
  )
  add_executable(unit_test ${TEST_SOURCES} ${SOURCES})
  target_compile_options(unit_test PRIVATE -O0)
  target_link_libraries(unit_test -pthread -lstdc++fs)
  add_test(NAME "unit_test_default"
    COMMAND unit_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
endif()
