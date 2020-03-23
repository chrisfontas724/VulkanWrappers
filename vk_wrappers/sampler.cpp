// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/sampler.hpp"

#include "vk_wrappers/logical_device.hpp"

namespace gfx {

SamplerPtr Sampler::create(LogicalDevicePtr device, bool lerp, bool unnormalized) {
    return std::shared_ptr<Sampler>(new Sampler(device, lerp, unnormalized));
}

Sampler::Sampler(LogicalDevicePtr device, bool lerp, bool unnormalized)
    : lerp_(lerp), unnormalized_(unnormalized) {
    vk::SamplerCreateInfo create_info(
        vk::SamplerCreateFlags(), lerp ? vk::Filter::eLinear : vk::Filter::eNearest,
        lerp ? vk::Filter::eLinear : vk::Filter::eNearest,
        lerp ? vk::SamplerMipmapMode::eLinear : vk::SamplerMipmapMode::eNearest,
        // According to the Vulkan spec, unnormalized texture coords cannot be
        // used with repeating sampler modes:
        // https://vulkan.lunarg.com/doc/view/1.0.26.0/linux/vkspec.chunked/ch12.html
        unnormalized ? vk::SamplerAddressMode::eClampToEdge : vk::SamplerAddressMode::eRepeat,
        unnormalized ? vk::SamplerAddressMode::eClampToEdge : vk::SamplerAddressMode::eRepeat,
        unnormalized ? vk::SamplerAddressMode::eClampToEdge : vk::SamplerAddressMode::eRepeat, 0,
        VK_TRUE && !unnormalized, 16, VK_FALSE, vk::CompareOp::eAlways, 0, 0,
        vk::BorderColor::eIntOpaqueBlack, unnormalized);
    sampler_ = device->vk().createSampler(create_info);
}

}  // namespace gfx