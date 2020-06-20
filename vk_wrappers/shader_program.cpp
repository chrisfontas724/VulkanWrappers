// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/shader_program.hpp"

namespace gfx {

ShaderProgramPtr ShaderProgram::createGraphics(const LogicalDevicePtr& device, const SpirV& vertex,
                                               const SpirV& fragment) {
    // Need at least a vertex shader.
    if (vertex.size() == 0) {
        CXL_DCHECK(false);
        return nullptr;
    }

    return std::shared_ptr<ShaderProgram>(new ShaderProgram(device,
     vk::PipelineBindPoint::eGraphics, 
    {{vk::ShaderStageFlagBits::eVertex, vertex}, {vk::ShaderStageFlagBits::eFragment, fragment}}));
}

ShaderProgram::ShaderProgram(const LogicalDevicePtr& device, const vk::PipelineBindPoint& bind_point,
                             const std::map<vk::ShaderStageFlagBits, SpirV>& spirv)
    : device_(device), bind_point_(bind_point) {
    std::vector<SpirV> spirv_vec;
    for (auto iter : spirv) {
        spirv_vec.push_back(iter.second);
        CXL_LOG(INFO) << "Making module: " << vk::to_string(iter.first);
        shader_modules_[iter.first] = std::make_unique<ShaderModule>(device, iter.first, iter.second);
    }

    reflection_ = std::make_unique<Reflection>(device_.lock(), spirv_vec);

    CXL_DCHECK(reflection_);
    layouts_ = reflection_->createLayouts();
    push_constants_ = reflection_->createPushConstants();

    std::vector<vk::DescriptorSetLayout> vk_layouts_;
    for (auto& layout : layouts_) {
        vk_layouts_.push_back(layout->vk());
    }

    // Pipeline layout.
    // TODO: Deal with push constants  more effectively.
    vk::PipelineLayoutCreateInfo pipeline_layout_info(
        {}, vk_layouts_.size(), vk_layouts_.data(), push_constants_.size(), push_constants_.data());

    try {
        pipeline_layout_ = device->vk().createPipelineLayout(pipeline_layout_info);
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    }
}

}  // namespace gfx
