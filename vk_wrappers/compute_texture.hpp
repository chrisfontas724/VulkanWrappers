// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_COMPUTE_TEXTURE_HPP_
#define GRAPHICS_VK_WRAPPERS_COMPUTE_TEXTURE_HPP_

#include <IlmBase/Half/half.h>

#include "forward_declarations.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "vk_wrappers/sampler.hpp"
#include "vk_wrappers/utils/image_utils.hpp"

namespace gfx {

class ComputeTexture {
   public:
    ComputeTexture(std::shared_ptr<LogicalDevice> device, const ImageUtils::Data& data);
    ComputeTexture(vk::ImageView view, vk::Image image, vk::ImageLayout layout, vk::Format format,
                   SamplerPtr sampler, uint32_t width, uint32_t height);
    ~ComputeTexture();

    vk::Sampler sampler() const { return sampler_->vk(); }

    vk::DescriptorType type() const { return vk::DescriptorType::eCombinedImageSampler; }

    vk::Image image() const { return image_; }

    vk::ImageView image_view() const { return image_view_; }

    vk::ImageLayout layout() const { return layout_; }

    vk::DescriptorImageInfo image_info() const {
        return vk::DescriptorImageInfo(sampler(), image_view(), layout());
    }

    void transitionImageLayout(CommandBuffer& command_buffer, vk::ImageLayout layout);

    uint32_t width() const { return width_; }

    uint32_t height() const { return height_; }

    uint32_t channels() const { return channels_; }

    uint32_t channel_size() const { return channel_size_; }

    vk::Format format() const { return format_; }

   private:
    LogicalDeviceWeakPtr device_;
    SamplerPtr sampler_;

    vk::ImageView image_view_;
    vk::Format format_;
    vk::Image image_;
    vk::DeviceMemory memory_;
    vk::ImageLayout layout_;
    uint32_t width_;
    uint32_t height_;
    uint32_t channels_;
    uint32_t channel_size_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMPUTE_TEXTURE_HPP_