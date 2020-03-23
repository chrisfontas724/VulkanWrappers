#include "graphics_pipeline.hpp"
#include "logging/logging.hpp"

namespace gfx {

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                                   const cxl::FileSystem* fs,
                                   const std::string& vertex_shader,
                                   const std::string& fragment_shader,
                                   std::vector<vk::VertexInputAttributeDescription> vertex_attributes,
                                   vk::VertexInputBindingDescription vertex_bindings,
                                   vk::PrimitiveTopology topology,
                                   vk::RenderPass render_pass,
                                   vk::Extent2D extent)

: Pipeline(device)
, topology_(topology)
, render_pass_(render_pass)
, extent_(extent) {
    CXL_DCHECK(fs);
    try {
        CXL_VLOG(7) << "Creating graphics pipeline (" << vertex_shader << ", " << fragment_shader << ")";
        vertex_shader_ = std::make_unique<ShaderModule>(device, fs, vertex_shader);
        fragment_shader_ = std::make_unique<ShaderModule>(device, fs, fragment_shader);
        auto spirv_code = { vertex_shader_->spir_v(), fragment_shader_->spir_v() };
        
        // Create reflection module.
        reflection_ = std::make_unique<Reflection>(device, spirv_code);
        CXL_DCHECK(reflection_) << "Could not generate reflection module.";

        initialize();
    } catch (...) {/*TODO*/}
}

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                                   const std::vector<uint32_t>& vertex_shader,
                                   const std::vector<uint32_t>& fragment_shader,
                                   std::vector<vk::VertexInputAttributeDescription> vertex_attributes,
                                   vk::VertexInputBindingDescription vertex_bindings,
                                   vk::PrimitiveTopology topology,
                                   vk::RenderPass render_pass,
                                   vk::Extent2D extent)
: Pipeline(device)
, topology_(topology)
, render_pass_(render_pass)
, extent_(extent) {
    vertex_shader_ = std::make_unique<ShaderModule>(device, vk::ShaderStageFlagBits::eVertex, vertex_shader);
    fragment_shader_ = std::make_unique<ShaderModule>(device, vk::ShaderStageFlagBits::eFragment, fragment_shader);
    auto spirv_code = { vertex_shader, fragment_shader };
        
    // Create reflection module.
    reflection_ = std::make_unique<Reflection>(device, spirv_code);
    CXL_DCHECK(reflection_) << "Could not generate reflection module.";

    CXL_LOG(INFO) << "initializing shader...!";
    initialize();
    CXL_LOG(INFO) << "finished shader initialization!";
}

void GraphicsPipeline::initialize() {
    auto device = device_.lock();

    vk::PipelineShaderStageCreateInfo shader_stages[] = {
        vertex_shader_->create_info(),
        fragment_shader_->create_info()
    };

    CXL_DCHECK(reflection_);
    layouts_ = reflection_->createLayouts();
    push_constants_ = reflection_->createPushConstants();

    // Vertex inputinfo
    vk::PipelineVertexInputStateCreateInfo vertex_input_info(
            {}, 0, nullptr, 0, nullptr);//1, &vertex_bindings, 
           // vertex_attributes.size(), vertex_attributes.data()
    // );

    // State create info.
    vk::PipelineInputAssemblyStateCreateInfo state_create_info({}, topology_);

    // Viewport.
    vk::Viewport viewport(0,0, extent_.width, extent_.height, 0.f, 1.f);

    // Scissor.
    vk::Rect2D scissor(vk::Offset2D(0,0), extent_);

    // Viewport state.
    vk::PipelineViewportStateCreateInfo viewport_state({}, 1, &viewport, 1, &scissor);

    vk::PipelineRasterizationStateCreateInfo rasterizer({}, 
        VK_FALSE, 
        VK_FALSE, 
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise,
        VK_FALSE, // Depth bias enable
        0,        // Depth bias constant factor
        0,        // Depth bias clamp
        0,        // Depth bias slope factor 
        1        // Line width
    );

    // Multi sampling - just use default.
    vk::PipelineMultisampleStateCreateInfo multisampling;

    // Color blend attachment. This is set to add so when multiple
    // passes are done for each ray bounce, the values accumulate.
    vk::PipelineColorBlendAttachmentState color_blend_attachment;
    color_blend_attachment.setBlendEnable(VK_TRUE);
    color_blend_attachment.setSrcColorBlendFactor(vk::BlendFactor::eOne);
    color_blend_attachment.setDstColorBlendFactor(vk::BlendFactor::eOne);
    color_blend_attachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne);
    color_blend_attachment.setDstAlphaBlendFactor(vk::BlendFactor::eOne);
    color_blend_attachment.setColorBlendOp(vk::BlendOp::eAdd);
    color_blend_attachment.setColorWriteMask(vk::ColorComponentFlagBits::eR |
                                             vk::ColorComponentFlagBits::eG |
                                             vk::ColorComponentFlagBits::eB |
                                             vk::ColorComponentFlagBits::eA);

    // Color blending.
     vk::PipelineColorBlendStateCreateInfo color_blending({}, 
        VK_FALSE, vk::LogicOp::eCopy, 1, &color_blend_attachment
    );

    std::vector<vk::DescriptorSetLayout> vk_layouts;
    for (auto& layout : layouts_) {
        vk_layouts.push_back(layout->vk());
    }

    // Pipeline layout.
    // TODO: Deal with push constants.
    vk::PipelineLayoutCreateInfo pipeline_layout_info({}, 
        vk_layouts.size(),
        vk_layouts.data(), 
        push_constants_.size(), 
        push_constants_.data()
    );

    pipeline_layout_ = device->vk().createPipelineLayout(pipeline_layout_info);

    // Dynamic State
    auto dynamic = vk::DynamicState::eViewport;
    vk::PipelineDynamicStateCreateInfo dynamic_state({}, 1, &dynamic);

    // Pipeline create info.
    vk::GraphicsPipelineCreateInfo pipeline_info(
        {}, 2, 
        shader_stages, 
        &vertex_input_info,
        &state_create_info,
        nullptr,
        &viewport_state,
        &rasterizer,
        &multisampling,
        nullptr, // DepthStencil State
        &color_blending,
        &dynamic_state, // Dynamic State
        pipeline_layout_,
        render_pass_,
        0,
        vk::Pipeline(),
        0
    );

    pipeline_ = device->vk().createGraphicsPipelines(vk::PipelineCache(), {pipeline_info})[0];
    CXL_VLOG(7) << "Finish creating graphics pipeline!\n\n";
}

GraphicsPipeline::~GraphicsPipeline() {
    vertex_shader_.reset();
    fragment_shader_.reset();

    auto device = device_.lock();
    device->vk().destroy(pipeline_layout_);
    device->vk().destroy(pipeline_);
}

} // gfx