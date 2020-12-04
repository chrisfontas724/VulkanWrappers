# Copyright 2019 Chris Fontas. All rights reserved.
# Use of this source code is governed by the license that can be
# found in the LICENSE file.

# Works with 3.11 and tested through 3.19
cmake_minimum_required(VERSION 3.11...3.19)

# Project name and a few useful settings. Other commands can pick up the results
project(
  VulkanWrappers
  VERSION 0.1
  DESCRIPTION "VK C++ Wrapper Classes"
  LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # C++ Compiler Settings.
    set (CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS ON)
    set(CMAKE_CXX_FLAGS "-std=gnu++17")
endif()


include_directories(/usr/include/)


# Find vulkan
find_package(Vulkan REQUIRED)


# Get GLM - This is a requirement so we fail early if it cannot be found.
set(GLM_INCLUDE_DIR C:/glm)
#find_package(glm REQUIRED)
#if(NOT glm_FOUND)
#    message(Error "GLM not found")
#endif(NOT glm_FOUND)
message(STATUS "Version1: GLM included at ${GLM_INCLUDE_DIR}")
message(STATUS "Version2: GLM included at ${glm_INCLUDE_DIR}")
message(STATUS ${GLM_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

set(CMAKE_C_COMPILE_OBJECT "${CMAKE_C_COMPILE_OBJECT}")


# Add externa projects
configure_file(CMakeLists.txt.in downloads/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/downloads)
if(result)
  message(FATAL_ERROR "CMake step for downloaded projects failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/downloads)
  
if(result)
  message(FATAL_ERROR "Build step for downloaded projects failed: ${result}")
endif()


include_directories(${CMAKE_CURRENT_BINARY_DIR}/filesystem-src
                    ${CMAKE_CURRENT_BINARY_DIR}/openexr-src
                    ${CMAKE_CURRENT_BINARY_DIR}/glslang-src
                    ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-src/include)


add_subdirectory(src)
