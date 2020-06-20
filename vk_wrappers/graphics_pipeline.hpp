// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_GRAPHICS_PIPELINE_HPP_
#define GRAPHICS_VK_WRAPPERS_GRAPHICS_PIPELINE_HPP_

#include "vk_wrappers/pipeline.hpp"

namespace gfx {
class GraphicsPipeline : public Pipeline {
   public:

    GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                     const ShaderModule* vertex_shader,
                     const ShaderModule* fragment_shader,
                     vk::PipelineLayout layout,
                     vk::PipelineRasterizationStateCreateInfo rasterizer,
                     vk::PipelineMultisampleStateCreateInfo multisampling,
                     vk::PipelineColorBlendStateCreateInfo color_blend_state,
                     std::vector<vk::VertexInputAttributeDescription> vertex_attributes,
                     vk::VertexInputBindingDescription vertex_bindings,
                     vk::PrimitiveTopology topology, vk::RenderPass render_pass,
                     vk::Extent2D extent);

    ~GraphicsPipeline();

    vk::PipelineBindPoint bind_point() override { return vk::PipelineBindPoint::eGraphics; }

   private:
    void initialize();
    const ShaderModule* vertex_shader_;
    const ShaderModule* fragment_shader_;
    std::vector<vk::VertexInputAttributeDescription> vertex_attributes_;
    vk::VertexInputBindingDescription vertex_bindings_;
    vk::PrimitiveTopology topology_;
    vk::RenderPass render_pass_;
    vk::Extent2D extent_;
    vk::PipelineRasterizationStateCreateInfo rasterizer_;
    vk::PipelineMultisampleStateCreateInfo multisampling_;
    vk::PipelineColorBlendStateCreateInfo color_blend_state_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_GRAPHICS_PIPELINE_HPP_
