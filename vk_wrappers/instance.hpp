// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_CONTEXT_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_CONTEXT_HPP_

#include <stdio.h>

#include <string>
#include <vector>

#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {

// Wrapper for vk::UniqueInstance. This is needed to instantiate Vulkan.
class Instance {
   public:
    // Construct an instance with a name |name|, list of extensions |extensions|
    // and with validation layers enabled or disabled with |validation|.
    static InstanceUnique create(const std::string& name,
                                 const std::vector<const char*>& extensions,
                                 bool validation = false);

    // Destructor.
    ~Instance();

    // Raw Vulkan handle.
    const vk::Instance& vk() const { return instance_.get(); }

    // Vector of all the physical devices found on the current platform.
    const std::vector<PhysicalDevicePtr>& physical_devices() const { return physical_devices_; }

    // Returns the physical device with the spec that is most optimal for the
    // passed in Vulkan surface.
    const PhysicalDevicePtr& pickBestDevice(const vk::SurfaceKHR& surface) const;

   private:
    explicit Instance(const std::string& name, const std::vector<const char*>& extensions,
                      bool validation);

    void locatePhysicalDevices();

    std::vector<std::shared_ptr<PhysicalDevice>> physical_devices_;
    vk::UniqueInstance instance_;
    vk::DebugUtilsMessengerEXT debugMessenger_;
    bool enable_validation_layers_;
};

}  // namespace gfx

#endif  // DALI_GRAPHICS_VK_WRAPPERS_CONTEXT_HPP_