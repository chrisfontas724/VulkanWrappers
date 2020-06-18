// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/command_buffer_state.hpp"

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

}  // namespace gfx
