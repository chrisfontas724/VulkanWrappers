// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/shader_pipeline.hpp"

namespace gfx {

ShaderPipeline::ShaderPipeline(const LogicalDevicePtr& device,
                               const std::vector<DescriptorSetLayoutPtr>& descriptor_layouts,
                               const std::vector<vk::PushConstantRange>& push_ranges) {
    uint32_t index = 0;
    std::vector<vk::DescriptorSetLayout> vk_layouts;
    for (auto& layout : descriptor_layouts) {
        vk_layouts.push_back(layout->vk());
        descriptor_set_mask_ |= 1 << index;
        index++;
    }

    // Pipeline layout.
    // TODO: Deal with push constants  more effectively.
    vk::PipelineLayoutCreateInfo pipeline_layout_info({}, vk_layouts.size(), vk_layouts.data(),
                                                      push_ranges.size(), push_ranges.data());

    try {
        layout_ = device->vk().createPipelineLayout(pipeline_layout_info);
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    }
}

}  // namespace gfx