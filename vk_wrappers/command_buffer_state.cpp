// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/command_buffer_state.hpp"
#include "vk_wrappers/shader_program.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "logging/logging.hpp"
#include "vk_wrappers/graphics_pipeline.hpp"

namespace gfx {


void CommandBufferState::set_default_state(DefaultState state) {

    // Common to all states.
    multisampling_.sampleShadingEnable = VK_FALSE;
    multisampling_.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling_.minSampleShading = 1.0f; // Optional
    multisampling_.pSampleMask = nullptr; // Optional
    multisampling_.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling_.alphaToOneEnable = VK_FALSE; // Optional


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
            vk::PipelineColorBlendAttachmentState color_blend_attachment;
            color_blend_attachment.setBlendEnable(VK_FALSE);
            color_blend_attachment.setColorWriteMask(
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

            // Color blending.
            color_blend_state_ = vk::PipelineColorBlendStateCreateInfo(
                {}, VK_FALSE, vk::LogicOp::eCopy, 1, &color_blend_attachment);
            break;
        }

        case DefaultState::kWireFrame:
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
            break;
        }
        case DefaultState::kNVRaytrace:
            break;
    };
}

void CommandBufferState::generateGraphicsPipeline(LogicalDevicePtr device) {
    CXL_DCHECK(device);
    CXL_DCHECK(shader_program_);

    graphics_pipeline_ = std::make_shared<gfx::GraphicsPipeline>(device,
                                                shader_program_->module(vk::ShaderStageFlagBits::eVertex),
                                                shader_program_->module(vk::ShaderStageFlagBits::eFragment),
                                                shader_program_->pipeline_layout(),
                                                rasterization_state_,
                                                multisampling_,
                                                color_blend_state_,
                                                std::vector<vk::VertexInputAttributeDescription>(),
                                                vk::VertexInputBindingDescription(),
                                                vk::PrimitiveTopology::eTriangleList,
                                                render_pass_,
                                                vk::Extent2D(1024, 768));
    CXL_LOG(INFO) << "FINISH CREATING GRAPHICS PIPELINE!!!";

//    vk::PipelineShaderStageCreateInfo shader_stages[] = {
//         vertex_module->pipeline_create_info(),
//         fragment_module->pipeline_create_info()
//     };

//     // Vertex inputinfo
//     // vk::PipelineVertexInputStateCreateInfo vertex_input_info(
//     //     /*flags*/{}, 
//     //     vertex_bindings_.size(), 
//     //     vertex_bindings_.data(), 
//     //     vertex_attributes_.size(),
//     //     vertex_attributes_.data());
//     // Vertex inputinfo
//     vk::PipelineVertexInputStateCreateInfo vertex_input_info(
//             {}, 0, nullptr, 0, nullptr);//1, &vertex_bindings, 
//            // vertex_attributes.size(), vertex_attributes.data()
//     // );    

//     // Input assembly.
//     vk::PipelineInputAssemblyStateCreateInfo input_assembly({}, topology_, VK_FALSE);

//     // Viewport state.
//        vk::PipelineShaderStageCreateInfo shader_stages[] = {
//         vertex_module->pipeline_create_info(),
//         fragment_module->pipeline_create_info()
//     };

//     // Vertex inputinfo
//     // vk::PipelineVertexInputStateCreateInfo vertex_input_info(
//     //     /*flags*/{}, 
//     //     vertex_bindings_.size(), 
//     //     vertex_bindings_.data(), 
//     //     vertex_attributes_.size(),
//     //     vertex_attributes_.data());
//     // Vertex inputinfo
//     vk::PipelineVertexInputStateCreateInfo vertex_input_info(
//             {}, 0, nullptr, 0, nullptr);//1, &vertex_bindings, 
//            // vertex_attributes.size(), vertex_attributes.data()
//     // );    

//     // Input assembly.
//     vk::PipelineInputAssemblyStateCreateInfo input_assembly({}, topology_, VK_FALSE);

//     // Viewport state.
//     vk::PipelineViewportStateCreateInfo viewport_state({}, 1, &viewport_, 1, &scissor_);

//     // Dynamic State
//     auto dynamic = vk::DynamicState::eViewport;
//     vk::PipelineDynamicStateCreateInfo dynamic_state({}, 1, &dynamic);

//     CXL_VLOG(7) << "    Topology: " << vk::to_string(topology_);
//     CXL_VLOG(7) << "    Viewport: " << viewport_.x << " " << viewport_.y << " " << viewport_.width << " " << viewport_.height << " " << viewport_.minDepth << " " << viewport_.maxDepth;
//     CXL_VLOG(7) << "    Scissor: " << scissor_.offset.x << " " << scissor_.offset.y << " " << scissor_.extent.width << " " << scissor_.extent.height;

//     vk::GraphicsPipelineCreateInfo pipeline_info;
//     pipeline_info.flags = {};
//     pipeline_info.stageCount = 2;
//     pipeline_info.pStages = shader_stages;
//     pipeline_info.pVertexInputState = &vertex_input_info;
//     pipeline_info.pInputAssemblyState = &input_assembly;
//     pipeline_info.pTessellationState = nullptr;
//     pipeline_info.pViewportState = &viewport_state;
//     pipeline_info.pRasterizationState = &rasterization_state_;
//     pipeline_info.pMultisampleState = &multisampling_;
//     pipeline_info.pDepthStencilState = nullptr;
//     pipeline_info.pColorBlendState = &color_blend_state_;
//     pipeline_info.pDynamicState = &dynamic_state;
//     pipeline_info.layout = shader_program_->pipeline_layout();
//     pipeline_info.renderPass = render_pass_;
//     pipeline_info.subpass = 0;
//     pipeline_info.basePipelineHandle = vk::Pipeline();
//     pipeline_info.basePipelineIndex = 0;

//     try {
//         pipeline_ = device->vk().createGraphicsPipelines(vk::PipelineCache(), {pipeline_info})[0];
//         CXL_VLOG(7) << "Finish creating graphics pipeline!\n\n";
//     } catch(vk::SystemError err) {
//         std::cout << "vk::SystemError: " << err.what() << std::endl;
//         exit(-1); 
//     }

    pipeline_ = graphics_pipeline_->vk();

}


}  // namespace gfx
