//
//  VkErrorHandling.cpp
//  VulkanTest
//
//  Created by FontasChristopher on 6/3/19.
//  Copyright © 2019 SicStudios. All rights reserved.
//
#include "vk_wrappers/error_handler.hpp"

#include <cstddef>
#include <fstream>
#include <glm/glm.hpp>
#include <optional>

namespace gfx {

std::string ErrorHandler::description(const VkResult& result) {
    switch (result) {
        case VK_SUCCESS:
            return "Success!";
        case VK_NOT_READY:
            return "Vulkan not ready";
        case VK_TIMEOUT:
            return "Vulkan timeout";
        case VK_EVENT_SET:
            return "Vulkan event set";
        case VK_EVENT_RESET:
            return "Vulkan event reset";
        case VK_INCOMPLETE:
            return "Vulkan incomplete";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "Vulkan out of host memory";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "Vulkan out of device memory";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "Vulkan initialization failed";
        case VK_ERROR_DEVICE_LOST:
            return "Vulkan device lost";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "Vulkan memory map failed";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "Vulkan layer not present";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "Vulkan extension not present";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "Vulkan feature not present";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "Vulkan incompatible driver";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "Vulkan too many objects";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "Vulkan format not suported";
        case VK_ERROR_FRAGMENTED_POOL:
            return "Vulkan error fragmented pool";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "Vulkan out of pool memory";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "Vulkan invalid external handle";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "Vulkan surface lost KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "Vulkan native window in use KHR";
        case VK_SUBOPTIMAL_KHR:
            return "Vulkan suboptimal KHR";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "Vulkan out of date KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "Vulkan incompatible display KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "Vulkan validation failed EXT";
        case VK_ERROR_INVALID_SHADER_NV:
            return "Vulkan Invalid Shader NV";
        case VK_ERROR_FRAGMENTATION_EXT:
            return "Vulkan fragmentation EXT";
        case VK_ERROR_NOT_PERMITTED_EXT:
            return "Vulkan not permitted EXT";
        case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
            return "Vulkan invalid device address EXT";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "Vulkan full screen exclusive mode lost EXT";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "Vulkan invalid DRM format modifier plane layout ext";
        case VK_RESULT_MAX_ENUM:
            return "Vulkan max enum";
        default:
            return "unknown";
    }
    return "";
}

}  // namespace gfx