// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_LOGICAL_DEVICE_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_LOGICAL_DEVICE_HPP_

#include <map>
#include <set>
#include <string>

#include "vk_wrappers/forward_declarations.hpp"
#include "vk_wrappers/queue.hpp"

namespace gfx {

// Wrapper around vk::LogicalDevice.
class LogicalDevice : public std::enable_shared_from_this<LogicalDevice> {
   public:
    LogicalDevice(const std::shared_ptr<PhysicalDevice>& physical_device,
                  const vk::SurfaceKHR& surface, const std::vector<const char*>& extensions = {});
    ~LogicalDevice();

    // Retrieve the physical device this logical device was created from.
    PhysicalDevicePtr physical_device() const { return physical_device_.lock(); }

    // Raw Vulkan handle.
    const vk::Device& vk() const { return device_.get(); }

    // Create vk::Semaphores.
    std::vector<vk::Semaphore> createSemaphores(const uint32_t& num,
                                                const vk::SemaphoreCreateFlags& flags = {}) const;
    vk::Semaphore createSemaphore(const vk::SemaphoreCreateFlags& flags = {}) const;

    // Create vk::Fences.
    std::vector<vk::Fence> createFences(const uint32_t& num,
                                        const vk::FenceCreateFlags& flags = {}) const;
    vk::Fence createFence(const vk::FenceCreateFlags& flags = {}) const;

    vk::CommandPool createCommandPool(Queue::Type type);

    vk::DescriptorSetLayout createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo info);

    uint32_t queue_family_index(Queue::Type type) const;

    const Queue& getQueue(Queue::Type type) const;

    // Waiting functions.
    void waitIdle() const;
    void waitForFence(const vk::Fence& fence);

    // Cleanup.
    template <typename T>
    void destroy(const T& t) const {
        device_->destroy(t);
    }

   private:
    std::weak_ptr<PhysicalDevice> physical_device_;
    vk::SurfaceKHR surface_;
    vk::UniqueDevice device_;
    std::map<Queue::Type, uint32_t> queue_families_map_;
    std::map<Queue::Type, std::unique_ptr<Queue>> queues_;
    std::vector<vk::CommandPool> command_pools_;
};
}  // namespace gfx

#endif  // DALI_GRAPHICS_VK_WRAPPERS_LOGICAL_DEVICE_HPP_