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
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_C_COMPILE_OBJECT "${CMAKE_C_COMPILE_OBJECT}")
endif()


# Find vulkan
find_package(Vulkan REQUIRED)
include_directories(${VUlkan_INCLUDE_DIRS})

# Find GLM
find_package(glm REQUIRED)
message(STATUS "Version1: GLM included at ${GLM_INCLUDE_DIRS}")
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})


# Add external projects
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


# This is so that a parent project that links this library knows
# which headers to include.
set(VK_INCLUDE_DIRS
   ${VK_INCLUDE_DIRS}	  
   ${CMAKE_CURRENT_BINARY_DIR}/filesystem-src	  
   ${CMAKE_CURRENT_BINARY_DIR}/openexr-src	
   ${CMAKE_CURRENT_BINARY_DIR}/glslang-src	
   ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-src	  
   PARENT_SCOPE
)	

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/glslang-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/glslang-build 
                 EXCLUDE_FROM_ALL)


add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/coreclasses-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-build 
                 EXCLUDE_FROM_ALL)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/filesystem-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/filesystem-build 
                 EXCLUDE_FROM_ALL)


add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/openexr-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/openexr-build 
                 EXCLUDE_FROM_ALL)

add_subdirectory(src)
add_subdirectory(apps)
