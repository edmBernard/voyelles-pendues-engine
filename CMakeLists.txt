project(vowels)
#**************************************************************************************************
# General cMake settings
#**************************************************************************************************
cmake_minimum_required(VERSION 3.5)

#**************************************************************************************************
# Find Package
#**************************************************************************************************
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)

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
  include/engine.h
)

#**************************************************************************************************
# Set compiler
#**************************************************************************************************
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_compile_options(/std:c++latest /Oy /utf-8)
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
add_library(vowels-engine STATIC ${SOURCES})
target_link_libraries(vowels-engine spdlog::spdlog fmt::fmt utf8cpp)

option(BUILD_EXEC "Build developper executable" OFF)
if(BUILD_EXEC)
  add_executable(devTool ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/tools/main.cpp)
  target_link_libraries(devTool -lstdc++fs spdlog::spdlog fmt::fmt utf8cpp)

  add_executable(devUTF8 ${CMAKE_CURRENT_SOURCE_DIR}/tools/main_utf8.cpp)
endif()

#**************************************************************************************************
# Test configuration
#**************************************************************************************************
option(BUILD_UNIT_TESTS "Build the unit tests" OFF)
if(BUILD_UNIT_TESTS)

  find_library(catch2 REQUIRED)
  find_path(CATCH_INCLUDE_DIR catch2/catch.hpp)
  include_directories(${CATCH_INCLUDE_DIR})

  enable_testing()
  SET(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-engine.cpp
  )
  add_executable(unit_test ${TEST_SOURCES} ${SOURCES})
  target_link_libraries(unit_test -pthread -lstdc++fs spdlog::spdlog fmt::fmt)
  add_custom_target(tests
    COMMAND unit_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

endif()
