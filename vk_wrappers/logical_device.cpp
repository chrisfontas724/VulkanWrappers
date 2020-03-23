// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/logical_device.hpp"
#include "vk_wrappers/physical_device.hpp"
#include "vk_wrappers/queue.hpp"
#include "logging/logging.hpp"
#include <iostream>

namespace gfx {

LogicalDevice::LogicalDevice(const std::shared_ptr<PhysicalDevice>& physical_device, 
                             const vk::SurfaceKHR& surface)
: physical_device_(physical_device)
, surface_(surface) {
    try {
        CXL_DCHECK(physical_device);
        auto indices = physical_device->queue_family_indices(surface);
        std::set<uint32_t> unique_queue_families = {indices.graphics_family.value(),
                                                  indices.present_family.value(),
                                                  indices.compute_family.value()};

        queue_families_map_ = {
            { Queue::Type::kGraphics, indices.graphics_family.value()},
            { Queue::Type::kCompute, indices.compute_family.value()},
            { Queue::Type::kPresent, indices.present_family.value()},
            { Queue::Type::kTransfer, indices.transfer_family.value()}
        };

        // Create queue creation structs.
        float queue_priority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        for (auto const& family : unique_queue_families) {
            CXL_VLOG(5) << "Creating queue with family index " << family;
            queue_create_infos.push_back(vk::DeviceQueueCreateInfo({}, family, 1, &queue_priority));
        }

        // Create a UniqueDevice.
        auto features = physical_device->features();
        std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        vk::DeviceCreateInfo info({}, queue_create_infos.size(),
                                      queue_create_infos.data(), 
                                      0, nullptr,
                                      1, extensions.data(), &features);
        device_ = physical_device->vk().createDeviceUnique(info);

        // Make queues.
        vk::Queue present_queue = device_->getQueue(queue_family_index(gfx::Queue::Type::kPresent), 0);
        vk::Queue graphics_queue = device_->getQueue(queue_family_index(gfx::Queue::Type::kGraphics), 0);
        vk::Queue compute_queue = device_->getQueue(queue_family_index(gfx::Queue::Type::kCompute), 0);
        vk::Queue transfer_queue = device_->getQueue(queue_family_index(gfx::Queue::Type::kTransfer), 0);

        queues_[gfx::Queue::Type::kPresent] = std::make_unique<gfx::Queue>(present_queue);
        queues_[gfx::Queue::Type::kGraphics] = std::make_unique<gfx::Queue>(graphics_queue);
        queues_[gfx::Queue::Type::kCompute] = std::make_unique<gfx::Queue>(compute_queue);
        queues_[gfx::Queue::Type::kTransfer] = std::make_unique<gfx::Queue>(transfer_queue);
    } catch(vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    } catch(...) {
        std::cout << "unknown error\n";
        exit(-1);
    }
}

LogicalDevice::~LogicalDevice() {
    for (auto& pool : command_pools_) {
        destroy(pool);
    }
    for (auto& queue : queues_) {
        queue.second.reset();
    }
    device_.reset();
}

std::vector<vk::Semaphore> LogicalDevice::createSemaphores(const uint32_t& num, const vk::SemaphoreCreateFlags& flags) const {
    std::vector<vk::Semaphore> result;
    for (uint32_t i = 0; i < num; i++) {
        result.push_back(createSemaphore(flags));
    }
    return result;
}

vk::Semaphore LogicalDevice::createSemaphore(const vk::SemaphoreCreateFlags& flags) const {
    return device_->createSemaphore(vk::SemaphoreCreateInfo(flags));
}


std::vector<vk::Fence> LogicalDevice::createFences(const uint32_t& num, const vk::FenceCreateFlags& flags) const {
    std::vector<vk::Fence> result;
    for (uint32_t i = 0; i < num; i++) {
        result.push_back(createFence(flags));
    }
    return result;
}

vk::Fence LogicalDevice::createFence(const vk::FenceCreateFlags& flags) const {
    return device_->createFence(vk::FenceCreateInfo(flags));
}

vk::CommandPool LogicalDevice::createCommandPool(Queue::Type type) {
    try {
        CXL_DCHECK(device_);
        vk::CommandPoolCreateFlags flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        vk::CommandPoolCreateInfo poolInfo(flags, queue_families_map_.at(type));
        command_pools_.push_back(device_->createCommandPool(poolInfo));
        return command_pools_[command_pools_.size() - 1];
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
    } catch (...) {
        std::cout << "unknown error\n";
    }
}

void LogicalDevice::waitForFence(const vk::Fence& fence) {
    device_->waitForFences(1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
}

uint32_t LogicalDevice::queue_family_index(Queue::Type type) const {
    return queue_families_map_.at(type); 
}

vk::DescriptorSetLayout LogicalDevice::createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo info) {
    return vk().createDescriptorSetLayout(info);
}

const Queue& LogicalDevice::getQueue(Queue::Type type) const {
    return *queues_.at(type).get();
}

void LogicalDevice::waitIdle() const {
    device_->waitIdle();
}

} // gfx
