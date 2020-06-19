// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/command_buffer_state.hpp"
#include "vk_wrappers/shader_program.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "logging/logging.hpp"

namespace gfx {

void CommandBufferState::set_default_state(DefaultState state) {
    switch (state) {
        case DefaultState::kOpaque:
        case DefaultState::kWireFrame:
        case DefaultState::kTranslucent:
        case DefaultState::kCustomRaytrace: {
            topology_ = vk::PrimitiveTopology::eLineList;
            rasterization_state_ = vk::PipelineRasterizationStateCreateInfo(
                {}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack,
                vk::FrontFace::eClockwise,
                VK_FALSE,  // Depth bias enable
                0,         // Depth bias constant factor
                0,         // Depth bias clamp
                0,         // Depth bias slope factor
                1          // Line width
            );

            // Color blend attachment. This is set to add so when multiple
            // passes are done for each ray bounce, the values accumulate.
            vk::PipelineColorBlendAttachmentState color_blend_attachment;
            color_blend_attachment.setBlendEnable(VK_TRUE);
            color_blend_attachment.setSrcColorBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment.setDstColorBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment.setDstAlphaBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment.setColorBlendOp(vk::BlendOp::eAdd);
            color_blend_attachment.setColorWriteMask(
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

            // Color blending.
            color_blend_state_ = vk::PipelineColorBlendStateCreateInfo(
                {}, VK_FALSE, vk::LogicOp::eCopy, 1, &color_blend_attachment);
        }
        case DefaultState::kNVRaytrace:
            break;
    };
}

vk::Pipeline CommandBufferState::generateGraphicsPipeline(LogicalDevicePtr device) {
    CXL_DCHECK(device);
    CXL_DCHECK(shader_program_);

    auto vertex_module = shader_program_->module(vk::ShaderStageFlagBits::eVertex);
    auto fragment_module = shader_program_->module(vk::ShaderStageFlagBits::eFragment);

    std::vector<vk::PipelineShaderStageCreateInfo> shader_stages = {
        vertex_module->pipeline_create_info(),
        fragment_module->pipeline_create_info()
    };

    // Vertex inputinfo
    vk::PipelineVertexInputStateCreateInfo vertex_input_info(
        /*flags*/{}, 
        vertex_bindings_.size(), 
        vertex_bindings_.data(), 
        vertex_attributes_.size(),
        vertex_attributes_.data());

    // Input assembly.
    vk::PipelineInputAssemblyStateCreateInfo input_assembly({}, topology_);

    // Viewport state.
    vk::PipelineViewportStateCreateInfo viewport_state({}, 1, &viewport_, 1, &scissor_);

    // Dynamic State
    auto dynamic = vk::DynamicState::eViewport;
    vk::PipelineDynamicStateCreateInfo dynamic_state({}, 1, &dynamic);

    vk::GraphicsPipelineCreateInfo pipeline_info(
        /*flags*/{},
        /*stageCount*/shader_stages.size(),
        /*pStages*/ shader_stages.data(),
        /*pVertexInputState*/&vertex_input_info,
        /*pInputAssemblyState*/&input_assembly,
        /*pTessellationState*/nullptr,
        /*pViewportState*/&viewport_state,
        /*pRasterizationState*/&rasterization_state_,
        /*pMultisampleState*/&multisampling_,
        /*pDepthStencilState*/&depth_stencil_,
        /*pColorBlendingState*/&color_blend_state_,
        /*pDynamicState*/&dynamic_state,
        /*pPipelineLayout*/shader_program_->pipeline_layout(),
        /*render_pass*/render_pass_,
        /*subpass*/0,
        /*pBasePipeline*/vk::Pipeline(),
        /*basePipelineIndex*/0
    );

    pipeline_ = device->vk().createGraphicsPipelines(vk::PipelineCache(), {pipeline_info})[0];
    CXL_VLOG(7) << "Finish creating graphics pipeline!\n\n";

    return pipeline_;
}


}  // namespace gfx
