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
  src/engine.cpp
)

#**************************************************************************************************
# Set compiler
#**************************************************************************************************
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_compile_options(/std:c++latest /Oy)
else()
  add_compile_options(-std=c++2a -fPIC -O3 -fno-omit-frame-pointer)
endif()

#**************************************************************************************************
# Linker
#**************************************************************************************************
LINK_DIRECTORIES(
)


#**************************************************************************************************
# Make configuration
#**************************************************************************************************
add_executable(devTool ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/tools/main.cpp)
target_compile_options(devTool PRIVATE -O3)
target_link_libraries(devTool -pthread -lstdc++fs)

#**************************************************************************************************
# Test configuration
#**************************************************************************************************
option(BUILD_UNIT_TESTS "Build the unit tests" ON)
if(BUILD_UNIT_TESTS)
  enable_testing()
  SET(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-engine.cpp
  )
  add_executable(unit_test ${TEST_SOURCES} ${SOURCES})
  target_link_libraries(unit_test -pthread -lstdc++fs)
  add_custom_target(tests
    COMMAND unit_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/datas
  )

endif()
