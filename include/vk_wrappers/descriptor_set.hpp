// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_DESCRIPTOR_SET_HPP_
#define GRAPHICS_VK_WRAPPERS_DESCRIPTOR_SET_HPP_

#include "vk_wrappers/compute_buffer.hpp"
#include "vk_wrappers/compute_texture.hpp"
#include "vk_wrappers/logical_device.hpp"

namespace gfx {
class DescriptorSetLayout;
class DescriptorSetAllocator;
class DescriptorSet {
   public:
    ~DescriptorSet();

    vk::DescriptorSet vk() const { return descriptor_set_; }

    void set_buffer(uint32_t index, std::shared_ptr<ComputeBuffer> buffer) const;

    void set_texture(uint32_t index, const ComputeTexturePtr& texture) const;

   private:
    friend class DescriptorSetLayout;

    DescriptorSet(std::shared_ptr<LogicalDevice> device, vk::DescriptorSet set,
                  vk::DescriptorSetLayoutCreateInfo info)
        : device_(device), descriptor_set_(set), create_info_(info) {}

    std::weak_ptr<LogicalDevice> device_;
    vk::DescriptorSet descriptor_set_;
    vk::DescriptorSetLayoutCreateInfo create_info_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_DESCRIPTOR_SET_HPP_