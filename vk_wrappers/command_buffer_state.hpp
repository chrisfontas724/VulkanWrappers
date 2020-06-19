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
    vk::RenderPass render_pass_;
    vk::Rect2D scissor_;
    vk::Viewport viewport_;
    vk::Extent2D extent_;
    vk::PrimitiveTopology topology_;
    std::vector<vk::VertexInputAttributeDescription> vertex_attributes_;

    vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_;
    vk::PipelineRasterizationStateCreateInfo rasterization_state_;
    vk::VertexInputBindingDescription vertex_bindings_;
    vk::PipelineMultisampleStateCreateInfo multisampling_;
    vk::PipelineDepthStencilStateCreateInfo depth_stencil_;
    vk::PipelineColorBlendStateCreateInfo color_blend_state_;
    bool has_recording_ = false;
    bool in_render_pass_ = false;
    PipelineWeakPtr pipeline_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_STATE_HPP_