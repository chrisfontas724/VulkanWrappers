# Copyright 2019 Chris Fontas. All rights reserved.
# Use of this source code is governed by the license that can be
# found in the LICENSE file.

set(LIBRARY_NAME vk_wrappers)

set(HEADER_LIST 
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/command_buffer_state.hpp" 
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/command_buffer.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/compute_buffer.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/compute_texture.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/image_view.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/image.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/instance.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/logical_device.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/physical_device.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/queue.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/render_pass.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/sampler.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/shader_compiler.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/shader_module.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/shader_program.hpp"
    "${VulkanWrappers_SOURCE_DIR}/include/vk_wrappers/swap_chain.hpp"
)

add_library (vk_wrappers
    command_buffer.cpp
    command_buffer_state.cpp
    compute_buffer.cpp
    compute_texture.cpp
    descriptor_allocator.cpp
    descriptor_set.cpp
    error_handler.cpp
    instance.cpp
    logical_device.cpp
    render_pass.cpp
    physical_device.cpp
    sampler.cpp
    shader_module.cpp
    shader_pipeline.cpp
    shader_program.cpp
    swap_chain.cpp
    queue.cpp
    image_utils.cpp
    shader_compiler.cpp
    reflection/reflection.cpp
    reflection/spirv_reflect.cpp
    ${HEADER_LIST}
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(vk_wrappers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link the dependencies.
target_link_libraries(vk_wrappers Vulkan::Vulkan) # glslang SPIRV streaming core)


# All users of this library will need at least C++11
target_compile_features(vk_wrappers PUBLIC cxx_std_11)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
