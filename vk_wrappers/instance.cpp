// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/instance.hpp"
#include "vk_wrappers/error_handler.hpp"
#include "vk_wrappers/physical_device.hpp"
#include "logging/logging.hpp"
#include <optional>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <map>
#include <set>
#include <cstddef>
#include <array>

namespace {

static const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
    
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

static bool checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        
        if (!layerFound) {
            return false;
        }
    }
    return true;
}

} // anonymous namespace

namespace gfx {

InstanceUnique Instance::create(const std::string& name, const std::vector<const char*>& extensions, bool validation) {
    try {
        return std::unique_ptr<Instance>(new Instance(name, extensions, validation));
    } catch (...) {
        std::cout << "Failed to create gfx::InstanceUnique!" << std::endl;
        throw;
    }
}

Instance::Instance(const std::string& name, const std::vector<const char*>& extensions, bool validation)
: enable_validation_layers_(validation) {
    if (enable_validation_layers_ && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    try {
        // Initialize the vk::ApplicationInfo structure.
        vk::ApplicationInfo applicationInfo(name.c_str(), 1, "christalz", 1, VK_API_VERSION_1_1);

        // Initialize the vk::InstanceCreateInfo.
        vk::InstanceCreateInfo instanceCreateInfo(
            /*flags*/ {},
            &applicationInfo, 
            validation ? validationLayers.size() : 0, 
            validation ? validationLayers.data() : nullptr, 
            extensions.size(), 
            extensions.data()
        );

        // Setup debug information.
        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if (validation) {
            debugCreateInfo.setPfnUserCallback(debugCallback);
            instanceCreateInfo.setPNext(&debugCreateInfo);
        }

        // Create a UniqueInstance.
        instance_ = vk::createInstanceUnique(instanceCreateInfo);

        // Get physical devices.
        locatePhysicalDevices();

     } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        throw;
    } catch (...) {
        std::cout << "unknown error\n";
        throw;
    }
}


Instance::~Instance() {
    for (auto& physical_device : physical_devices_) {
        physical_device.reset();
    }
}

void Instance::locatePhysicalDevices() {
    std::vector<vk::PhysicalDevice> physical_devices = instance_->enumeratePhysicalDevices();
    for(auto& device : physical_devices) {
        auto physical_device =  std::shared_ptr<PhysicalDevice>(new PhysicalDevice(device));
        physical_device->printDiagnostics();
        physical_devices_.push_back(std::move(physical_device));
    }
}

const std::shared_ptr<PhysicalDevice>& Instance::pickBestDevice(const vk::SurfaceKHR& surface) const {
    // Use an ordered map to automatically sort candidates by increasing score.
    std::multimap<uint32_t, const std::shared_ptr<PhysicalDevice>> candidates;
        
    for (const auto& device : physical_devices_) {
        uint32_t score = device->performanceScore(surface);
        candidates.insert(std::make_pair(score, std::move(device)));
    }
        
    // Check if the best candidate is suitable at all.
    if (candidates.rbegin()->first > 0) {
        return candidates.rbegin()->second;
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

} // gfx
