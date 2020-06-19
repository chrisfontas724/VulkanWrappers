// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/shader_program.hpp"

namespace gfx {

ShaderProgram::ShaderProgram(const LogicalDevicePtr& device, const vk::PipelineBindPoint& bind_point,
                             const std::map<vk::ShaderStageFlagBits, SpirV>& spirv)
    : device_(device), bind_point_(bind_point) {
    std::vector<SpirV> spirv_vec;
    for (auto iter : spirv) {
        auto module = std::make_unique<ShaderModule>(device, iter.first, iter.second);
        spirv_vec.push_back(iter.second);
    }

    reflection_ = std::make_unique<Reflection>(device_.lock(), spirv_vec);

    CXL_DCHECK(reflection_);
    layouts_ = reflection_->createLayouts();
    push_constants_ = reflection_->createPushConstants();

    std::vector<vk::DescriptorSetLayout> vk_layouts;
    for (auto& layout : layouts_) {
        vk_layouts.push_back(layout->vk());
    }

    // Pipeline layout.
    // TODO: Deal with push constants.
    vk::PipelineLayoutCreateInfo pipeline_layout_info(
        {}, vk_layouts.size(), vk_layouts.data(), push_constants_.size(), push_constants_.data());

    pipeline_layout_ = device->vk().createPipelineLayout(pipeline_layout_info);
}

}  // namespace gfx
