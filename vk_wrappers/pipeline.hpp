// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_PIPELINE_HPP_
#define GRAPHICS_VK_WRAPPERS_PIPELINE_HPP_

#include "vk_wrappers/forward_declarations.hpp"
#include "vk_wrappers/shader_module.hpp"
#include "vk_wrappers/utils/reflection/reflection.hpp"

namespace gfx {

class Pipeline {
   public:
    Pipeline(LogicalDevicePtr device_) : device(device_) {}

    vk::Pipeline vk() const { return pipeline_; }

    vk::PipelineLayout pipeline_layout() const { return pipeline_layout_; }

    std::shared_ptr<DescriptorSetLayout> layout(uint32_t index) const {
        return (index < layouts_.size()) ? layouts_[index] : nullptr;
    }

    vk::PushConstantRange push_constant(uint32_t index) const {
        CXL_DCHECK(index < push_constants_.size());
        return push_constants_[index];
    }

    virtual vk::PipelineBindPoint bind_point() = 0;

   protected:
    LogicalDevicePtr device;
    vk::Pipeline pipeline_;
    vk::PipelineLayout pipeline_layout_;
    std::vector<std::shared_ptr<DescriptorSetLayout>> layouts_;
    std::vector<vk::PushConstantRange> push_constants_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_PIPELINE_HPP_