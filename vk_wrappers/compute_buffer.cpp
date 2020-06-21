// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/compute_buffer.hpp"

#include "vk_wrappers/command_buffer.hpp"
#include "vk_wrappers/physical_device.hpp"

namespace gfx {

namespace {
uint32_t findMemoryType(std::shared_ptr<PhysicalDevice> physical_device, uint32_t typeFilter,
                        vk::MemoryPropertyFlags properties) {
    vk::PhysicalDeviceMemoryProperties memProperties = physical_device->vk().getMemoryProperties();
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}
}  // namespace

ComputeBuffer::ComputeBuffer(std::shared_ptr<LogicalDevice> device, vk::DeviceSize size,
                             vk::BufferUsageFlags usage, vk::DescriptorType type,
                             vk::MemoryPropertyFlags properties)
    : device_(device), size_(size), usage_(usage), type_(type), properties_(properties) {
    try {
        vk::BufferCreateInfo info({}, size, usage);  //, properties)
        buffer_ = device->vk().createBuffer(info);

        vk::MemoryRequirements mem_requirements = device->vk().getBufferMemoryRequirements(buffer_);
        vk::MemoryAllocateInfo allocate_info(
            size,
            findMemoryType(device->physical_device(), mem_requirements.memoryTypeBits, properties));
        buffer_memory_ = device->vk().allocateMemory(allocate_info);

        // Now associate that allocated memory with the buffer.
        device->vk().bindBufferMemory(buffer_, buffer_memory_, 0);

        is_valid_ = true;
    } catch (...) {
    }
}

void ComputeBuffer::copyBuffer(std::shared_ptr<ComputeBuffer> source, vk::DeviceSize size) {
    auto device = device_.lock();
    auto command_buffer = gfx::CommandBuffer::create(device, gfx::Queue::Type::kTransfer,
                                                     vk::CommandBufferLevel::ePrimary);
    const auto& queue = device->getQueue(Queue::Type::kTransfer);

    vk::BufferCopy copy_region;
    copy_region.srcOffset = 0;  // Optional
    copy_region.dstOffset = 0;  // Optional
    copy_region.size = size;

    command_buffer->reset();
    command_buffer->beginRecording();
    command_buffer->vk().copyBuffer(source->vk(), vk(), 1, &copy_region);
    command_buffer->endRecording();
    queue.submit(command_buffer);
    queue.waitIdle();
    command_buffer.reset();
}

ComputeBuffer::~ComputeBuffer() {
    if (is_valid_) {
        auto device = device_.lock();
        CXL_DCHECK(device);

        device->vk().destroyBuffer(buffer_);
        device->vk().freeMemory(buffer_memory_);
    }
}

}  // namespace gfx