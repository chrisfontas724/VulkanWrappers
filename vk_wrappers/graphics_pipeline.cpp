#include "graphics_pipeline.hpp"

#include "logging/logging.hpp"

namespace gfx {

GraphicsPipeline::GraphicsPipeline(
    std::shared_ptr<LogicalDevice> device, const ShaderModule* vertex_shader,
    const ShaderModule* fragment_shader, vk::PipelineLayout layout,
    vk::PipelineRasterizationStateCreateInfo rasterizer,
    vk::PipelineMultisampleStateCreateInfo multisampling,
    vk::PipelineColorBlendStateCreateInfo color_blend_state,
    std::vector<vk::VertexInputAttributeDescription> vertex_attributes,
    vk::VertexInputBindingDescription vertex_bindings, vk::PrimitiveTopology topology,
    vk::RenderPass render_pass, vk::Extent2D extent)
    : Pipeline(device),
      vertex_shader_(vertex_shader),
      fragment_shader_(fragment_shader),
      rasterizer_(rasterizer),
      multisampling_(multisampling),
      color_blend_state_(color_blend_state),
      vertex_attributes_(vertex_attributes),
      vertex_bindings_(vertex_bindings),
      topology_(topology),
      render_pass_(render_pass),
      extent_(extent) {
    pipeline_layout_ = layout;
    initialize();
}

void GraphicsPipeline::initialize() {
    vk::PipelineShaderStageCreateInfo shader_stages[] = {vertex_shader_->pipeline_create_info(),
                                                         fragment_shader_->pipeline_create_info()};

    // Vertex inputinfo
    vk::PipelineVertexInputStateCreateInfo vertex_input_info(
        {}, 1, &vertex_bindings_, vertex_attributes_.size(), vertex_attributes_.data());

    // State create info.
    vk::PipelineInputAssemblyStateCreateInfo input_assembly({}, topology_, VK_FALSE);

    // Viewport.
    vk::Viewport viewport(0, 0, extent_.width, extent_.height, 0.f, 1.f);

    // Scissor.
    vk::Rect2D scissor(vk::Offset2D(0, 0), extent_);

    // Viewport state.
    vk::PipelineViewportStateCreateInfo viewport_state({}, 1, &viewport, 1, &scissor);

    // Color blend attachment. This is set to add so when multiple
    // passes are done for each ray bounce, the values accumulate.
    vk::PipelineColorBlendAttachmentState color_blend_attachment;
    color_blend_attachment.setBlendEnable(VK_FALSE);
    color_blend_attachment.setColorWriteMask(
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    // Color blending.
    auto color_blending = vk::PipelineColorBlendStateCreateInfo({}, VK_FALSE, vk::LogicOp::eCopy, 1,
                                                                &color_blend_attachment);

    // Dynamic State
    auto dynamic = vk::DynamicState::eViewport;
    vk::PipelineDynamicStateCreateInfo dynamic_state({}, 1, &dynamic);

    // Pipeline create info.
    vk::GraphicsPipelineCreateInfo pipeline_info(
        {}, 2, shader_stages, &vertex_input_info, &input_assembly, nullptr, &viewport_state,
        &rasterizer_, &multisampling_,
        nullptr,  // DepthStencil State
        &color_blending,
        &dynamic_state,  // Dynamic State
        pipeline_layout_, render_pass_, 0, vk::Pipeline(), 0);

    pipeline_ = device->vk().createGraphicsPipelines(vk::PipelineCache(), {pipeline_info}).value[0];
    CXL_VLOG(7) << "Finish creating graphics pipeline!\n\n";
}

GraphicsPipeline::~GraphicsPipeline() {
    device->vk().destroy(pipeline_layout_);
    device->vk().destroy(pipeline_);
}

}  // namespace gfx