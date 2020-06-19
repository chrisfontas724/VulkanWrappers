// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
#define GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_

#include "vk_wrappers/forward_declarations.hpp"
#include "vk_wrappers/shader_module.hpp"
#include "vk_wrappers/utils/reflection/reflection.hpp"

namespace gfx {

class ShaderProgram {
   public:
    static ShaderProgramPtr createGraphics(const LogicalDevicePtr& device, const SpirV& vertex,
                                           const SpirV& fragment);

    static ShaderProgramPtr createCompute(const LogicalDevicePtr& device, const SpirV& kernel);

    vk::PipelineLayout pipeline_layout() const { return pipeline_layout_; }

    std::shared_ptr<DescriptorSetLayout> layout(uint32_t index) const {
        return (index < layouts_.size()) ? layouts_[index] : nullptr;
    }

    vk::PushConstantRange push_constant(uint32_t index) const {
        CXL_DCHECK(index < push_constants_.size());
        return push_constants_[index];
    }

    vk::PipelineBindPoint bind_point() const { return bind_point_; }

    const ShaderModule* module(vk::ShaderStageFlagBits stage) const {
        return shader_modules_.at(stage).get();
    }

   private:
    ShaderProgram(const LogicalDevicePtr& device, const vk::PipelineBindPoint& bind_point,
                  const std::map<vk::ShaderStageFlagBits, SpirV>& spirv);

        LogicalDeviceWeakPtr device_;
    std::unique_ptr<Reflection> reflection_;
    vk::PipelineBindPoint bind_point_;
    vk::PipelineLayout pipeline_layout_;
    std::vector<std::shared_ptr<DescriptorSetLayout>> layouts_;
    std::vector<vk::PushConstantRange> push_constants_;
    std::map<vk::ShaderStageFlagBits, std::unique_ptr<ShaderModule>> shader_modules_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
