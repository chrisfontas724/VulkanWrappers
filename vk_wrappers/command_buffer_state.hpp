// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_STATE_HPP_
#define GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_STATE_HPP_

#include "vk_wrappers/forward_declarations.hpp"
#include "vk_wrappers/vulkan.hpp"

namespace gfx {

class CommandBufferState {
   public:
    enum class DefaultState {
        kOpaque,
        kWireFrame,
        kTranslucent,
        kNVRaytrace,
        kCustomRaytrace,
    };

    void set_default_state(DefaultState state);

   private:
    friend class CommandBuffer;

    struct VertexBinding {
        vk::Format formats[8];
        uint32_t offsets[8];
    };

    struct VertexDescription {
        VertexBinding bindings[8];
    };

    struct BindCall {
        const ComputeBuffer* buffer;
    };

    void generateGraphicsPipeline(LogicalDevicePtr device);

    ShaderProgramPtr shader_program_;
    vk::RenderPass render_pass_;
    vk::Rect2D scissor_;
    vk::Viewport viewport_;
    vk::PrimitiveTopology topology_;

    std::vector<vk::VertexInputAttributeDescription> vertex_attributes_;
    std::vector<vk::VertexInputBindingDescription> vertex_bindings_;

    vk::PipelineRasterizationStateCreateInfo rasterization_state_;
    vk::PipelineMultisampleStateCreateInfo multisampling_;
    vk::PipelineDepthStencilStateCreateInfo depth_stencil_;
    vk::PipelineColorBlendAttachmentState color_blend_attachment_;
    bool has_recording_ = false;
    bool in_render_pass_ = false;
    uint32_t num_subpasses_;
    uint32_t current_subpass_;
    GraphicsPipelinePtr graphics_pipeline_;
    vk::Pipeline pipeline_;
    vk::PipelineCache pipeline_cache_;
    VertexDescription vertex_description_;
    // std::vector<BindCall> bind_calls_;
    BindCall bind_call_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_STATE_HPP_