// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/shader_pipeline.hpp"

#include "utils/hasher.hpp"

namespace gfx {

ShaderPipeline::ShaderPipeline(const LogicalDevicePtr& device,
                               const std::vector<DescriptorSetLayoutPtr>& descriptor_layouts,
                               const std::vector<vk::PushConstantRange>& push_ranges)
: device_(device) {
    uint32_t index = 0;
    std::vector<vk::DescriptorSetLayout> vk_layouts;
    for (auto& layout : descriptor_layouts) {
        descriptor_layouts_[index] = layout;
        vk_layouts.push_back(layout->vk());
        descriptor_set_mask_ |= 1 << index;
        index++;
    }

    // Pipeline layout.
    // TODO: Deal with push constants  more effectively.
    layout_info_ = vk::PipelineLayoutCreateInfo({}, vk_layouts.size(), vk_layouts.data(),
                                                push_ranges.size(), push_ranges.data());

    try {
        layout_ = device->vk().createPipelineLayout(layout_info_);
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    }
}

ShaderPipeline::~ShaderPipeline() {
    auto device = device_.lock();
    CXL_DCHECK(device);
    CXL_VLOG(5) << "Deleting shader pipeline!";
    device->vk().destroyPipelineLayout(layout_);
    CXL_VLOG(5) << "Finished deleting shader pipeline!";
}

uint32_t ShaderPipeline::push_constant_layout_hash() const {
    cxl::Hasher hasher(0);
    for (const auto& range : push_ranges_) {
        hasher.hash(range.offset);
        hasher.hash(range.size);
    }
    return hasher.get_hash();
}

}  // namespace gfx