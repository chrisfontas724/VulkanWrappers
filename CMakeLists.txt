cmake_minimum_required (VERSION 3.7)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "-std=gnu++17")


add_library (vulkan_wrappers
    vk_wrappers/command_buffer.cpp
    vk_wrappers/compute_buffer.cpp
    vk_wrappers/compute_pipeline.cpp
    vk_wrappers/graphics_pipeline.cpp
    vk_wrappers/compute_texture.cpp
    vk_wrappers/descriptor_allocator.cpp
    vk_wrappers/descriptor_set.cpp
    vk_wrappers/error_handler.cpp
    vk_wrappers/frame_buffer.cpp
    vk_wrappers/instance.cpp
    vk_wrappers/logical_device.cpp
    vk_wrappers/physical_device.cpp
    vk_wrappers/sampler.cpp
    vk_wrappers/shader_module.cpp
    vk_wrappers/swap_chain.cpp
    vk_wrappers/queue.cpp
    vk_wrappers/utils/image_utils.cpp
    vk_wrappers/utils/render_pass_utils.cpp
    vk_wrappers/utils/shader_compiler.cpp
    vk_wrappers/utils/reflection/reflection.cpp
    vk_wrappers/utils/reflection/spirv_reflect.c
)

find_package(Vulkan REQUIRED)
target_link_libraries(vulkan_wrappers Vulkan::Vulkan glslang SPIRV)
target_include_directories(vulkan_wrappers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

