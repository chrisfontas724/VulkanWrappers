cmake_minimum_required (VERSION 3.7)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "-std=gnu++17")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/filesystem-src
                    ${CMAKE_CURRENT_BINARY_DIR}/openexr-src
                    ${CMAKE_CURRENT_BINARY_DIR}/glslang-src
                    ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-src)

# Add filesystem classes
configure_file(CMakeLists.txt.in filesystem-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/filesystem-download )
if(result)
  message(FATAL_ERROR "CMake step for filesystem failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/filesystem-download )
if(result)
  message(FATAL_ERROR "Build step for filesystem failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/filesystem-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/filesystem-build 
                 EXCLUDE_FROM_ALL)

# Add core classes
configure_file(CMakeLists.txt.in coreclasses-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-download )
if(result)
  message(FATAL_ERROR "CMake step for filesystem failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-download )
if(result)
  message(FATAL_ERROR "Build step for filesystem failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/coreclasses-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-build 
                 EXCLUDE_FROM_ALL)

# Add gslang directory
configure_file(CMakeLists.txt.in glslang-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glslang-download )
if(result)
  message(FATAL_ERROR "CMake step for glslang failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glslang-download )
if(result)
  message(FATAL_ERROR "Build step for glslang failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/glslang-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/glslang-build 
                 EXCLUDE_FROM_ALL)

# Add openexr classes
configure_file(CMakeLists.txt.in openexr-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/openexr-download )
if(result)
  message(FATAL_ERROR "CMake step for openexr failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/openexr-download )
if(result)
  message(FATAL_ERROR "Build step for openexr failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/openexr-src 
                 ${CMAKE_CURRENT_BINARY_DIR}/openexr-build 
                 EXCLUDE_FROM_ALL)

add_library (vulkan_wrappers
    vk_wrappers/command_buffer.cpp
    vk_wrappers/command_buffer_state.cpp
    vk_wrappers/compute_buffer.cpp
    vk_wrappers/graphics_pipeline.cpp
    vk_wrappers/compute_texture.cpp
    vk_wrappers/descriptor_allocator.cpp
    vk_wrappers/descriptor_set.cpp
    vk_wrappers/error_handler.cpp
    vk_wrappers/instance.cpp
    vk_wrappers/logical_device.cpp
    vk_wrappers/render_pass.cpp
    vk_wrappers/physical_device.cpp
    vk_wrappers/sampler.cpp
    vk_wrappers/shader_module.cpp
    vk_wrappers/shader_program.cpp
    vk_wrappers/swap_chain.cpp
    vk_wrappers/queue.cpp
    vk_wrappers/utils/image_utils.cpp
    vk_wrappers/utils/shader_compiler.cpp
    vk_wrappers/utils/reflection/reflection.cpp
    vk_wrappers/utils/reflection/spirv_reflect.c
)

find_package(Vulkan REQUIRED)

# This is so that a parent project that links this library knows
# which headers to include.
set(VK_INCLUDE_DIRS
   ${VK_INCLUDE_DIRS}
   ${CMAKE_CURRENT_BINARY_DIR}/filesystem-src
   ${CMAKE_CURRENT_BINARY_DIR}/openexr-src
   ${CMAKE_CURRENT_BINARY_DIR}/glslang-src
   ${CMAKE_CURRENT_BINARY_DIR}/coreclasses-src
   /usr/include/
   PARENT_SCOPE
)

# Get GLM - This is a requirement so we fail early if it cannot be found.
find_package(glm REQUIRED)
if(NOT glm_FOUND)
    message(Error "GLM not found")
endif(NOT glm_FOUND)


message(STATUS "Version1: GLM included at ${GLM_INCLUDE_DIR}")
message(STATUS "Version2: GLM included at ${glm_INCLUDE_DIR}")
message(STATUS ${GLM_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

include_directories(/usr/include/)

# Link the dependencies.
target_link_libraries(vulkan_wrappers Vulkan::Vulkan glslang SPIRV streaming core)
target_include_directories(vulkan_wrappers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

