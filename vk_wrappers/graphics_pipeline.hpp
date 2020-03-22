// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_GRAPHICS_PIPELINE_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_GRAPHICS_PIPELINE_HPP_

#include "vk_wrappers/pipeline.hpp"

namespace gfx{
class GraphicsPipeline : public Pipeline {
public:

    GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                     const cxl::FileSystem* fs,
                     const std::string& vertex_shader,
                     const std::string& fragment_shader,
                     std::vector<vk::VertexInputAttributeDescription> vertex_attributes,
                     vk::VertexInputBindingDescription vertex_bindings,
                     vk::PrimitiveTopology topology,
                     vk::RenderPass render_pass,
                     vk::Extent2D extent);

    GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                     const std::vector<uint32_t>& vertex_shader,
                     const std::vector<uint32_t>& fragment_shader,
                     std::vector<vk::VertexInputAttributeDescription> vertex_attributes,
                     vk::VertexInputBindingDescription vertex_bindings,
                     vk::PrimitiveTopology topology,
                     vk::RenderPass render_pass,
                     vk::Extent2D extent);

    ~GraphicsPipeline();

   vk::PipelineBindPoint bind_point() override { return vk::PipelineBindPoint::eGraphics; }

private:

    void initialize();

    std::unique_ptr<Reflection> reflection_;
    std::unique_ptr<ShaderModule> vertex_shader_;
    std::unique_ptr<ShaderModule> fragment_shader_;
    vk::PrimitiveTopology topology_;
    vk::RenderPass render_pass_;
    vk::Extent2D extent_;
};

} // gfx

#endif // DALI_GRAPHICS_VK_WRAPPERS_GRAPHICS_PIPELINE_HPP_
