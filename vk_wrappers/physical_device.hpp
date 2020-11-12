// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_PHYSICAL_DEVICE_HPP_
#define GRAPHICS_VK_WRAPPERS_PHYSICAL_DEVICE_HPP_

#include <optional>
#include <string>

#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {

// Wrapper class around vk::PhysicalDevice which simplifies
// the interface for ease of use.
class PhysicalDevice {
   public:
    // Struct to store the queue indices that are used to
    // create the queues used by the logical device.
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> compute_family;
        std::optional<uint32_t> present_family;
        std::optional<uint32_t> transfer_family;

        bool isComplete() {
            return graphics_family.has_value() && present_family.has_value() &&
                   compute_family.has_value() && transfer_family.has_value();
        }
    };

    // Information about the kind of support this particular
    // device has for swapchains.
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> present_modes;
    };

    // Dumps all the relevant information about the device
    // to the log, including its vendor information, API
    // version, list of features and limitations.
    void printDiagnostics();

    // This is used to determine how good the device is.
    // The higher the score the better.
    uint32_t performanceScore(const vk::SurfaceKHR& surface,
                              const std::vector<const char*>& extensions = {}) const;

    // Returns the index of the requested memory type.
    uint32_t findMemoryType(uint32_t type_filter, vk::MemoryPropertyFlags mem_properties);

    vk::Format findDepthFormat() const;

    // Getter for the queue family indices.
    QueueFamilyIndices queue_family_indices(const vk::SurfaceKHR& surface) const {
        return findQueueFamilies(surface);
    }

    // Get a raw reference to the underlying vulkan type.
    const vk::PhysicalDevice& vk() const { return physical_device_; }

    // Get swap chain support information.
    SwapChainSupportDetails querySwapChainSupport(const vk::SurfaceKHR& surface) const;

    vk::SampleCountFlagBits maximumMSAA() const;

    const std::string name() const { return properties_.properties.deviceName; }

    const vk::PhysicalDeviceFeatures features() const { return features_; }

   private:
    friend class Instance;

    // Constructor - Must take in a raw vulkan physical device.
    // Only |Instance| can instantiate a physical device.
    explicit PhysicalDevice(vk::PhysicalDevice physical_device);

    // Populates the QueueFamilyIndices struct.
    QueueFamilyIndices findQueueFamilies(const vk::SurfaceKHR& surface) const;

    vk::PhysicalDevice physical_device_;
    vk::PhysicalDeviceProperties2 properties_;
    vk::PhysicalDeviceRayTracingPropertiesNV ray_trace_properties_;
    vk::PhysicalDeviceFeatures features_;
    std::vector<vk::QueueFamilyProperties> queue_family_properties_;
    std::vector<vk::LayerProperties> layer_properties_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_PHYSICAL_DEVICE_HPP_