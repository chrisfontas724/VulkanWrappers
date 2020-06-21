// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_COMPUTE_BUFFER_HPP_
#define GRAPHICS_VK_WRAPPERS_COMPUTE_BUFFER_HPP_

#include "logging/logging.hpp"
#include "vk_wrappers/logical_device.hpp"

namespace gfx {
class ComputeBuffer {
   public:
    static std::shared_ptr<ComputeBuffer> createSourceBuffer(std::shared_ptr<LogicalDevice> device,
                                                             vk::DeviceSize size) {
        auto flags = vk::BufferUsageFlagBits::eTransferSrc;
        auto type = vk::DescriptorType::eStorageBuffer;
        auto memory =
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
        return std::shared_ptr<ComputeBuffer>(new ComputeBuffer(device, size, flags, type, memory));
    }

    static std::shared_ptr<ComputeBuffer> createHostAccessableBuffer(
        std::shared_ptr<LogicalDevice> device, vk::DeviceSize size) {
        auto flags =
            vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst;
        auto type = vk::DescriptorType::eStorageBuffer;
        auto memory =
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
        return std::shared_ptr<ComputeBuffer>(new ComputeBuffer(device, size, flags, type, memory));
    }

    static std::shared_ptr<ComputeBuffer> createHostAccessableUniform(
        std::shared_ptr<LogicalDevice> device, vk::DeviceSize size) {
        auto flags =
            vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst;
        auto type = vk::DescriptorType::eUniformBuffer;
        auto memory =
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
        return std::shared_ptr<ComputeBuffer>(new ComputeBuffer(device, size, flags, type, memory));
    }

    static std::shared_ptr<ComputeBuffer> createStorageBuffer(std::shared_ptr<LogicalDevice> device,
                                                              vk::DeviceSize size) {
        auto flags = vk::BufferUsageFlagBits::eStorageBuffer;
        auto type = vk::DescriptorType::eStorageBuffer;
        auto memory = vk::MemoryPropertyFlagBits::eDeviceLocal;
        return std::shared_ptr<ComputeBuffer>(new ComputeBuffer(device, size, flags, type, memory));
    }

    static std::shared_ptr<ComputeBuffer> createUniformBuffer(std::shared_ptr<LogicalDevice> device,
                                                              vk::DeviceSize size) {
        auto flags = vk::BufferUsageFlagBits::eUniformBuffer;
        auto type = vk::DescriptorType::eUniformBuffer;
        auto memory = vk::MemoryPropertyFlagBits::eDeviceLocal;
        return std::shared_ptr<ComputeBuffer>(new ComputeBuffer(device, size, flags, type, memory));
    }

    template <typename T>
    static std::shared_ptr<ComputeBuffer> createFromVector(std::shared_ptr<LogicalDevice> device,
                                                           const std::vector<T>& input,
                                                           vk::BufferUsageFlagBits usage_flags) {
        vk::DeviceSize buffer_size = sizeof(input[0]) * input.size();
        auto flags = vk::BufferUsageFlagBits::eTransferSrc;
        auto type = vk::DescriptorType::eStorageBuffer;
        auto memory =
            vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
        auto staging_buffer = std::shared_ptr<ComputeBuffer>(
            new ComputeBuffer(device, buffer_size, flags, type, memory));

        staging_buffer->access<T>(0, [&input, &buffer_size](T* data) {
            memcpy(data, input.data(), static_cast<uint64_t>(buffer_size));
        });

        auto dst_flags = vk::BufferUsageFlagBits::eTransferDst | usage_flags;
        auto dst_type = vk::DescriptorType::eStorageBuffer;
        auto dst_memory = vk::MemoryPropertyFlagBits::eDeviceLocal;
        auto dst_buffer = std::shared_ptr<ComputeBuffer>(
            new ComputeBuffer(device, buffer_size, dst_flags, dst_type, dst_memory));

        // TODO: Copy over from staging buffer
        dst_buffer->copyBuffer(staging_buffer, buffer_size);

        staging_buffer.reset();
        return dst_buffer;
    }

    template <typename T>
    uint64_t num_elements() {
        return static_cast<uint64_t>(size_ / sizeof(T));
    }

    ~ComputeBuffer();

    void copyBuffer(std::shared_ptr<ComputeBuffer> source, vk::DeviceSize size);

    vk::Buffer vk() const { return buffer_; }
    vk::DeviceSize size() const { return size_; }
    vk::DescriptorType type() const { return type_; }
    vk::BufferUsageFlags usage() const { return usage_; }
    vk::MemoryPropertyFlags properties() const { return properties_; }

    // Read or write to the command buffer via a mapped host pointer,
    // which can be templatized to any given type.
    template <typename T>
    void access(vk::DeviceSize offset, std::function<void(T*)> func) {
        CXL_DCHECK(offset < size_);
        CXL_DCHECK(properties_ != vk::MemoryPropertyFlagBits::eDeviceLocal)
            << "Cannot access device-local memory on the host";

        auto device = device_.lock();
        CXL_DCHECK(device);

        void* data;
        device->vk().mapMemory(buffer_memory_, offset, size_ - offset, {}, &data);
        CXL_DCHECK(data);

        func(static_cast<T*>(data));

        device->vk().unmapMemory(buffer_memory_);
    }

    // Convenience wrapper around |access| that writes the specified
    // data to the buffer.
    template <typename T>
    void write(const T* input, uint64_t num, uint64_t offset = 0) {
        access<T>(sizeof(T) * offset, [&](T* data) {
            CXL_DCHECK(input && data);
            CXL_DCHECK(num > 0);
            memcpy(data, input, sizeof(T) * num);
        });
    }

    // Convenience wrapper around |access| that reads from the buffer
    // back into host memory.
    template <typename T>
    void read(T* output, uint64_t num, uint64_t offset = 0) {
        access<T>(sizeof(T) * offset, [&](T* data) {
            CXL_DCHECK(output && data);
            CXL_DCHECK(num > 0);
            memcpy(output, data, sizeof(T) * num);
        });
    }

    template <typename T>
    void read(std::vector<T>& output) {
        uint32_t num = num_elements<T>();
        output.resize(num);
        read<T>(output.data(), num, 0);
    }

   private:
    ComputeBuffer(std::shared_ptr<LogicalDevice> device, vk::DeviceSize size,
                  vk::BufferUsageFlags usage, vk::DescriptorType type,
                  vk::MemoryPropertyFlags properties);

    std::weak_ptr<LogicalDevice> device_;
    vk::DeviceSize size_;
    vk::BufferUsageFlags usage_;
    vk::DescriptorType type_;
    vk::MemoryPropertyFlags properties_;

    vk::Buffer buffer_;
    vk::DeviceMemory buffer_memory_;

    bool is_valid_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMPUTE_BUFFER_HPP_