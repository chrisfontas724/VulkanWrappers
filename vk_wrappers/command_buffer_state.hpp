// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_STATE_HPP_
#define GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_STATE_HPP_

#include <map>

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
        VertexBinding() {
            for (uint32_t i = 0; i < 8; i++) {
                formats[i] = vk::Format::eUndefined;
            }
        }
    };

    struct VertexDescription {
        VertexBinding bindings[8];
    };

    struct BindCall {
        std::vector<vk::Buffer> buffers;
        std::vector<vk::DeviceSize> offsets;
        vk::Buffer index_buffer;
    };

    struct DescriptorBinding {
        DescriptorBinding() {}
        union {
            vk::DescriptorBufferInfo buffer_info;
            vk::DescriptorImageInfo image_info;
        };
        uint64_t identifier;
        vk::DescriptorType type;
    };

    struct DescriptorSet {
        DescriptorBinding bindings[8];
    };

    struct PipelineResources {
        DescriptorSet descriptors[32];
        uint8_t push_constants[256];
    };

    void generateGraphicsPipeline(LogicalDevicePtr device);
    void generateComputePipeline(LogicalDevicePtr device);

    std::pair<std::vector<vk::VertexInputBindingDescription>,
              std::vector<vk::VertexInputAttributeDescription>>
    createVertexBindingsAndAttributes();

    ShaderProgramPtr shader_program_;
    vk::RenderPass render_pass_;
    vk::Rect2D scissor_;
    vk::Viewport viewport_;
    vk::PrimitiveTopology topology_;

    vk::PipelineRasterizationStateCreateInfo rasterization_state_;
    vk::PipelineMultisampleStateCreateInfo multisampling_;
    vk::PipelineDepthStencilStateCreateInfo depth_stencil_;
    vk::PipelineColorBlendAttachmentState color_blend_attachment_;
    bool has_recording_ = false;
    bool in_render_pass_ = false;
    uint32_t num_subpasses_;
    uint32_t current_subpass_;
    vk::SampleCountFlagBits msaa_samples_;
    GraphicsPipelinePtr graphics_pipeline_;
    vk::Pipeline pipeline_;
    vk::PipelineCache pipeline_cache_;
    VertexDescription vertex_description_;
    BindCall bind_call_;
    std::map<uint32_t, vk::Pipeline> pipeline_hash_;
    std::map<uint32_t, vk::DescriptorSet> descriptor_hash_;
    PipelineResources pipeline_resources_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_STATE_HPP_