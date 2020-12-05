// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_SHADER_PIPELINE_HPP_
#define GRAPHICS_VK_WRAPPERS_SHADER_PIPELINE_HPP_

#include "descriptor_allocator.hpp"
#include "forward_declarations.hpp"
#include "shader_module.hpp"
#include "reflection.hpp"

namespace gfx {

class ShaderPipeline {
   public:
    ShaderPipeline(const LogicalDevicePtr& device,
                   const std::vector<DescriptorSetLayoutPtr>& descriptor_layouts,
                   const std::vector<vk::PushConstantRange>& push_ranges);

    ~ShaderPipeline();

    uint32_t descriptor_set_mask() const { return descriptor_set_mask_; }

    DescriptorSetLayout* descriptor_layout(uint32_t index) const {
        //CXL_DCHECK(descriptor_set_mask_ & (1 << index)) << index;
        return descriptor_layouts_[index].get();
    }

    vk::PipelineLayout vk() const { return layout_; }

    const std::vector<vk::PushConstantRange>& push_ranges() const { return push_ranges_; }

    uint32_t push_constant_layout_hash() const;

   private:
    LogicalDeviceWeakPtr device_;
    DescriptorSetLayoutPtr descriptor_layouts_[32];
    vk::PipelineLayoutCreateInfo layout_info_;
    vk::PipelineLayout layout_;
    uint32_t descriptor_set_mask_ = 0u;
    std::vector<vk::PushConstantRange> push_ranges_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_SHADER_PIPELINE_HPP_