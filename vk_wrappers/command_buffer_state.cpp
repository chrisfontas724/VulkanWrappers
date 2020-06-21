// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/command_buffer_state.hpp"

#include "logging/logging.hpp"
#include "vk_wrappers/graphics_pipeline.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "vk_wrappers/shader_program.hpp"

namespace {

std::map<vk::Format, uint32_t> format_to_size_map = {
    {vk::Format::eR32G32B32A32Sfloat, 16U},
};

}  // namespace

namespace gfx {

void CommandBufferState::set_default_state(DefaultState state) {
    // Common to all states.
    multisampling_.sampleShadingEnable = VK_FALSE;
    multisampling_.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling_.minSampleShading = 1.0f;           // Optional
    multisampling_.pSampleMask = nullptr;             // Optional
    multisampling_.alphaToCoverageEnable = VK_FALSE;  // Optional
    multisampling_.alphaToOneEnable = VK_FALSE;       // Optional

    // Color blending.
    auto color_blend_state = vk::PipelineColorBlendStateCreateInfo({}, VK_FALSE, vk::LogicOp::eCopy,
                                                                   1, &color_blend_attachment_);

    switch (state) {
        case DefaultState::kOpaque: {
            topology_ = vk::PrimitiveTopology::eTriangleList;
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
            color_blend_attachment_.setBlendEnable(VK_FALSE);
            color_blend_attachment_.setColorWriteMask(
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
            break;
        }
        case DefaultState::kWireFrame: {
            topology_ = vk::PrimitiveTopology::eTriangleList;
            rasterization_state_ = vk::PipelineRasterizationStateCreateInfo(
                {}, VK_FALSE, VK_FALSE, vk::PolygonMode::eLine, /*cull mode*/ {},
                vk::FrontFace::eClockwise,
                VK_FALSE,  // Depth bias enable
                0,         // Depth bias constant factor
                0,         // Depth bias clamp
                0,         // Depth bias slope factor
                1          // Line width
            );

            // Color blend attachment. This is set to add so when multiple
            // passes are done for each ray bounce, the values accumulate.
            color_blend_attachment_.setBlendEnable(VK_FALSE);
            color_blend_attachment_.setColorWriteMask(
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
            break;
        }
        case DefaultState::kTranslucent:
        case DefaultState::kCustomRaytrace: {
            topology_ = vk::PrimitiveTopology::ePointList;
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
            color_blend_attachment_.setBlendEnable(VK_TRUE);
            color_blend_attachment_.setSrcColorBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment_.setDstColorBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment_.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment_.setDstAlphaBlendFactor(vk::BlendFactor::eOne);
            color_blend_attachment_.setColorBlendOp(vk::BlendOp::eAdd);
            color_blend_attachment_.setColorWriteMask(
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
            break;
        }
        case DefaultState::kNVRaytrace:
            break;
    };
}

void CommandBufferState::generateGraphicsPipeline(LogicalDevicePtr device) {
    CXL_DCHECK(device);
    CXL_DCHECK(shader_program_);

    auto vertex_module = shader_program_->module(vk::ShaderStageFlagBits::eVertex);
    auto fragment_module = shader_program_->module(vk::ShaderStageFlagBits::eFragment);
    vk::PipelineShaderStageCreateInfo shader_stages[] = {vertex_module->pipeline_create_info(),
                                                         fragment_module->pipeline_create_info()};

    vertex_bindings_.clear();
    vertex_attributes_.clear();
    for (uint32_t i = 0; i < 8; i++) {
        auto binding = vertex_description_.bindings[i];
        vk::VertexInputBindingDescription input_binding;
        input_binding.binding = i;
        input_binding.inputRate = vk::VertexInputRate::eVertex;
        uint32_t stride = 0;
        for (uint32_t j = 0; j < 8; j++) {
            auto format = binding.formats[j];
            auto offset = binding.offsets[j];
            if (format == vk::Format()) {
                break;
            }

            vk::VertexInputAttributeDescription input_attribute;
            input_attribute.binding = i;
            input_attribute.location = j;
            input_attribute.format = format;
            input_attribute.offset = offset;
            vertex_attributes_.push_back(input_attribute);

            stride += format_to_size_map[format];
        }
        if (stride == 0) {
            continue;
        }
        input_binding.stride = stride;
        vertex_bindings_.push_back(input_binding);
        break;
    }

    // Vertex inputinfo
    vk::PipelineVertexInputStateCreateInfo vertex_input_info(
        /*flags*/ {}, vertex_bindings_.size(), vertex_bindings_.data(), vertex_attributes_.size(),
        vertex_attributes_.data());

    // Input assembly.
    vk::PipelineInputAssemblyStateCreateInfo input_assembly({}, topology_, VK_FALSE);

    // Viewport state.
    vk::PipelineViewportStateCreateInfo viewport_state({}, 1, &viewport_, 1, &scissor_);

    // Dynamic State
    auto dynamic = vk::DynamicState::eViewport;
    vk::PipelineDynamicStateCreateInfo dynamic_state({}, 1, &dynamic);

    // Color blending.
    auto color_blend_state = vk::PipelineColorBlendStateCreateInfo({}, VK_FALSE, vk::LogicOp::eCopy,
                                                                   1, &color_blend_attachment_);

    vk::GraphicsPipelineCreateInfo pipeline_info;
    pipeline_info.flags = {};
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pTessellationState = nullptr;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterization_state_;
    pipeline_info.pMultisampleState = &multisampling_;
    pipeline_info.pDepthStencilState = &depth_stencil_,
    pipeline_info.pColorBlendState = &color_blend_state;
    pipeline_info.pDynamicState = &dynamic_state;
    pipeline_info.layout = shader_program_->pipeline_layout();
    pipeline_info.renderPass = render_pass_;
    pipeline_info.subpass = current_subpass_;
    pipeline_info.basePipelineHandle = vk::Pipeline();
    pipeline_info.basePipelineIndex = 0;

    try {
        pipeline_ = device->vk().createGraphicsPipelines(pipeline_cache_, {pipeline_info})[0];
        CXL_VLOG(7) << "Finish creating graphics pipeline!\n\n";
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    }
}

}  // namespace gfx
