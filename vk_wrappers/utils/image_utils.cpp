// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/utils/image_utils.hpp"
#include "vk_wrappers/command_buffer.hpp"
#include "vk_wrappers/physical_device.hpp"

namespace gfx {

namespace {
void createImage(std::shared_ptr<LogicalDevice> device,
                 uint32_t width, uint32_t height, 
                 vk::Format format, 
                 vk::ImageTiling tiling,
                 vk::ImageUsageFlags usage,
                 vk::MemoryPropertyFlags properties, 
                 vk::Image& image, 
                 vk::ImageLayout layout,
                 vk::DeviceMemory& image_memory) {
    vk::ImageCreateInfo create_info;
    create_info.setImageType(vk::ImageType::e2D);
    create_info.setExtent(vk::Extent3D(width, height,1));
    create_info.setMipLevels(1);
    create_info.setArrayLayers(1);
    create_info.setFormat(format);
    create_info.setTiling(tiling);
    create_info.setInitialLayout(layout);
    create_info.setUsage(usage);
    create_info.setSamples(vk::SampleCountFlagBits::e1);
    create_info.setSharingMode(vk::SharingMode::eExclusive);

    device->vk().createImage(&create_info, nullptr, &image);

    vk::MemoryRequirements mem_requirements = device->vk().getImageMemoryRequirements(image);
    vk::MemoryAllocateInfo memory_info(mem_requirements.size, 
                                       device->physical_device()->findMemoryType(mem_requirements.memoryTypeBits,
                                                                                 properties));
    device->vk().allocateMemory(&memory_info, nullptr, &image_memory);

    device->vk().bindImageMemory(image, image_memory, 0);
}

template<typename T>
ImageUtils::Data createImageAbstract(std::shared_ptr<LogicalDevice> device,
                                     uint32_t width, 
                                     uint32_t height,
                                     uint32_t channels,
                                     uint32_t data_size,
                                     vk::Format format,
                                     vk::ImageUsageFlags usage,
                                     vk::ImageLayout src_layout,
                                     vk::ImageLayout dst_layout,
                                     const T* pixels) {
    CXL_VLOG(3) << "CreateImageAbstract: " << data_size;
    auto command_buffer = gfx::CommandBuffer::create(device, gfx::Queue::Type::kTransfer, vk::CommandBufferLevel::ePrimary);
    const auto& queue = device->getQueue(Queue::Type::kTransfer);

    CXL_VLOG(3) << "Create staging buffer!";
    // First create staging buffer, and copy the pixels to it.
    uint64_t size = width * height * data_size * channels;
    std::shared_ptr<ComputeBuffer> staging_buffer = nullptr; 
    if (pixels) {
        staging_buffer = ComputeBuffer::createSourceBuffer(device, size);
        staging_buffer->write<T>(pixels, width*height*channels);
    }

    // Next create image and memory.
    vk::Image image;
    vk::DeviceMemory memory;
    createImage(device, width, height, 
                format, 
                vk::ImageTiling::eOptimal, 
                usage,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                image, vk::ImageLayout::eUndefined, memory);
    CXL_VLOG(3) << "Created base image!";

    // Transition image layout.
    command_buffer->reset();
    command_buffer->beginRecording();
    command_buffer->transitionImageLayout(image, format, vk::ImageLayout::eUndefined, src_layout);

    if (pixels) {
        // Copy buffer.
        CXL_VLOG(3) << "Copy pixels!";
        command_buffer->copyBufferToImage(staging_buffer, image, width, height);
    }

    if (src_layout != dst_layout) {
        // Transition image layout.
        command_buffer->transitionImageLayout(image, format, src_layout, dst_layout);
    }

    command_buffer->endRecording();
    queue.submit(command_buffer);
    queue.waitIdle();

    command_buffer.reset();

    // Return data.
    CXL_VLOG(3) << "Returning image abstract data!";
    return { .image = image, 
             .memory = memory, 
             .format = format, 
             .extent = vk::Extent2D(width, height), 
             .layout = dst_layout
    };
}

} // anonymous namespace


ImageUtils::Data ImageUtils::create8BitUnormImage(std::shared_ptr<LogicalDevice> device,
                                                  uint32_t width,
                                                  uint32_t height,
                                                  uint32_t channels,
                                                  const uint8_t* pixels) {
    auto src_layout = vk::ImageLayout::eTransferDstOptimal;
    auto dst_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
    auto usage =vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

    vk::Format format;
    switch (channels) {
        case 4:
            format = vk::Format::eR8G8B8A8Unorm;
            break;
        case 3:
            format = vk::Format::eR8G8B8Unorm;
            break;
        case 1:
            format = vk::Format::eR8Uint;
            break;
        default:
            CXL_DCHECK(false);
            break;
    };

    return createImageAbstract<uint8_t>(device, width, height, 4, sizeof(uint8_t), format, usage, src_layout, dst_layout, pixels);
}

ImageUtils::Data ImageUtils::createHDRImage(std::shared_ptr<LogicalDevice> device, uint32_t width, uint32_t height, const half* pixels) {
    auto src_layout = vk::ImageLayout::eTransferDstOptimal;
    auto dst_layout = vk::ImageLayout::eShaderReadOnlyOptimal;
    auto usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
    return createImageAbstract<half>(device, width, height, 4, sizeof(half), vk::Format::eR16G16B16A16Sfloat, usage, src_layout, dst_layout, pixels);
}

ImageUtils::Data ImageUtils::createColorAttachment(std::shared_ptr<LogicalDevice> device, uint32_t width, uint32_t height) {
    auto usage = vk::ImageUsageFlagBits::eTransferSrc |
                vk::ImageUsageFlagBits::eTransferDst |
                vk::ImageUsageFlagBits::eColorAttachment | 
                vk::ImageUsageFlagBits::eSampled;
    auto layout = vk::ImageLayout::eColorAttachmentOptimal;
    return createImageAbstract<uint8_t>(device, width, height, 4, sizeof(uint8_t), vk::Format::eB8G8R8A8Unorm, usage, layout, layout, nullptr);
}

ImageUtils::Data ImageUtils::createAccumulationAttachment(std::shared_ptr<LogicalDevice> device, uint32_t width, uint32_t height) {
    auto usage = vk::ImageUsageFlagBits::eTransferSrc |
                vk::ImageUsageFlagBits::eTransferDst |
                vk::ImageUsageFlagBits::eColorAttachment | 
                vk::ImageUsageFlagBits::eSampled;
    auto layout = vk::ImageLayout::eColorAttachmentOptimal;
    return createImageAbstract<float>(device, width, height, 4, sizeof(float), vk::Format::eR32G32B32A32Sfloat, usage, layout, layout, nullptr);   
}

vk::ImageView ImageUtils::createImageView(std::shared_ptr<LogicalDevice> device, const Data& data) {
    return createImageView(device, data.image, data.format);
}

vk::ImageView ImageUtils::createImageView(std::shared_ptr<LogicalDevice> device,
                                          const vk::Image& image,
                                          const vk::Format& format) {
    vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor,0,1,0,1);
    vk::ImageViewCreateInfo info({}, 
                                 image,
                                 vk::ImageViewType::e2D,
                                 format,
                                 vk::ComponentMapping(), 
                                 range);
    return device->vk().createImageView(info);
} 
} // gfx