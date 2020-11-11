// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/render_pass.hpp"

#include "logging/logging.hpp"

namespace gfx {

RenderPassBuilder::RenderPassBuilder(LogicalDevicePtr device) : device_(device) {}

void RenderPassBuilder::addColorAttachment(ComputeTexturePtr texture, AttachmentInfo info,
                                           glm::vec4 clear_value) {
    bool is_swapchain_image = texture->layout() == vk::ImageLayout::ePresentSrcKHR;
    vk::ImageLayout final_layout = is_swapchain_image ? vk::ImageLayout::ePresentSrcKHR
                                                      : vk::ImageLayout::eColorAttachmentOptimal;

    CXL_CHECK(!(is_swapchain_image && texture->samples() != vk::SampleCountFlagBits::e1));

    CXL_VLOG(3) << "Is swapchain image: " << is_swapchain_image;
    CXL_VLOG(3) << "FORMAT: " << vk::to_string(texture->format());
    vk::AttachmentDescription attachment(
        /*flags*/ {},
        /*format*/ texture->format(),
        /*samples*/ texture->samples(),
        /*loadOp*/ info.load_op,
        /*storeOp*/ info.store_op,
        /*stencilLoadOp*/ vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/ vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/ vk::ImageLayout::eUndefined,
        /*finalLayout*/ final_layout);

    color_attachments_.push_back(attachment);
    color_textures_.push_back(texture);
    color_clear_values_.push_back(clear_value);
}

void RenderPassBuilder::addResolveAttachment(ComputeTexturePtr texture, AttachmentInfo info) {
    CXL_CHECK(texture->layout() == vk::ImageLayout::ePresentSrcKHR);
    CXL_DCHECK(texture->samples() == vk::SampleCountFlagBits::e1);
    vk::AttachmentDescription attachment(
        /*flags*/ {},
        /*format*/ texture->format(),
        /*samples*/ texture->samples(),
        /*loadOp*/ info.load_op,
        /*storeOp*/ info.store_op,
        /*stencilLoadOp*/ vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/ vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/ vk::ImageLayout::eUndefined,
        /*finalLayout*/ vk::ImageLayout::ePresentSrcKHR);

    resolve_attachments_.push_back(attachment);
    resolve_textures_.push_back(texture);
    color_clear_values_.push_back(glm::vec4(0));
}

void RenderPassBuilder::addDepthAttachment(ComputeTexturePtr texture, AttachmentInfo info,
                                           glm::vec2 clear_value) {
    vk::AttachmentDescription attachment(
        /*flags*/ {},
        /*format*/ texture->format(),
        /*samples*/ texture->samples(),
        /*loadOp*/ info.load_op,
        /*storeOp*/ info.store_op,
        /*stencilLoadOp*/ vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/ vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/ vk::ImageLayout::eUndefined,
        /*finalLayout*/ vk::ImageLayout::eDepthStencilAttachmentOptimal);

    depth_attachments_.push_back(attachment);
    depth_textures_.push_back(texture);
    depth_clear_values_.push_back(clear_value);
}

void RenderPassBuilder::addSubpass(SubpassInfo info) { subpasses_.push_back(info); }

RenderPassInfo RenderPassBuilder::build() {
    auto device = device_.lock();
    CXL_DCHECK(device);

    const uint32_t kNumCol = color_attachments_.size();
    const uint32_t kNumDepth = depth_attachments_.size();
    CXL_CHECK(kNumCol >= 1);

    std::vector<vk::AttachmentDescription> all_attachments = color_attachments_;
    all_attachments.insert(all_attachments.end(), depth_attachments_.begin(),
                           depth_attachments_.end());
    all_attachments.insert(all_attachments.end(), resolve_attachments_.begin(),
                           resolve_attachments_.end());

    std::vector<vk::SubpassDescription> subpasses;

    struct SubpassData {
        std::vector<vk::AttachmentReference> input_references;
        std::vector<vk::AttachmentReference> color_references;
        vk::AttachmentReference depth_reference;
        vk::AttachmentReference resolve_reference;
    };

    std::vector<SubpassData> subpass_data;
    subpass_data.resize(subpasses_.size());

    vk::SubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.srcAccessMask = {};
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

    uint32_t subpass_index = 0;
    for (const auto& subpass_info : subpasses_) {
        auto& input_references = subpass_data[subpass_index].input_references;
        auto& color_references = subpass_data[subpass_index].color_references;
        auto& depth_reference = subpass_data[subpass_index].depth_reference;
        auto& resolve_reference = subpass_data[subpass_index].resolve_reference;

        for (uint32_t index : subpass_info.input_indices) {
            const auto& attachment = color_attachments_[index];
            input_references.push_back(vk::AttachmentReference(index, attachment.finalLayout));
        }

        for (uint32_t index : subpass_info.color_indices) {
            CXL_CHECK(index < color_attachments_.size());
            const auto& attachment = color_attachments_[index];
            color_references.push_back(
                vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal));
        }

        // Depth textures are not required.
        if (subpass_info.depth_index.has_value()) {
            uint32_t index = *subpass_info.depth_index;
            CXL_CHECK(index < depth_attachments_.size());
            const auto& depth_attachment = depth_attachments_[index];
            depth_reference =
                vk::AttachmentReference(kNumCol + index, depth_attachment.finalLayout);
        }

        // Resolve textures are not required.
        if (subpass_info.resolve_index.has_value()) {
            uint32_t index = *subpass_info.resolve_index;
            const auto& resolve_attachment = resolve_attachments_[index];
            CXL_VLOG(3) << "Resolve index: " << kNumCol + kNumDepth + index;
            resolve_reference = vk::AttachmentReference(kNumCol + kNumDepth + index,
                                                        vk::ImageLayout::eColorAttachmentOptimal);
        }

        vk::SubpassDescription subpass(
            /*flags*/ {},
            /*bindPoint*/ subpass_info.bind_point,
            /*num input*/ input_references.size(),
            /*inputs*/ input_references.size() > 0 ? input_references.data() : nullptr,
            /*num color*/ color_references.size(),
            /*color refs*/ color_references.data(),
            /*resolve ref*/ subpass_info.resolve_index.has_value() ? &resolve_reference : nullptr,
            /*depth ref*/ subpass_info.depth_index.has_value() ? &depth_reference : nullptr);

        subpasses.push_back(subpass);
        subpass_index++;
    }

    vk::RenderPassCreateInfo render_pass_info(
        /*flags*/ {},
        /*attachment_count*/ all_attachments.size(),
        /*attachment_data*/ all_attachments.data(),
        /*subpass_count*/ subpasses.size(),
        /*subpass_data*/ subpasses.data());

    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    vk::RenderPass render_pass = device->vk().createRenderPass(render_pass_info);
    uint32_t width = color_textures_[0]->width();
    uint32_t height = color_textures_[0]->height();

    std::vector<const ComputeTexture*> textures;
    std::vector<vk::ImageView> image_views;
    for (const auto& texture : color_textures_) {
        image_views.push_back(texture->image_view());
        textures.push_back(texture.get());
    }
    for (const auto& texture : depth_textures_) {
        image_views.push_back(texture->image_view());
        textures.push_back(texture.get());
    }
    for (const auto& texture : resolve_textures_) {
        image_views.push_back(texture->image_view());
        textures.push_back(texture.get());
    }

    std::vector<vk::ClearValue> clear_values;
    for (auto val : color_clear_values_) {
        vk::ClearColorValue clear_val(std::array<float, 4>{val.x, val.y, val.z, val.w});
        clear_values.push_back(clear_val);
    }

    for (auto val : depth_clear_values_) {
        vk::ClearDepthStencilValue clear_val(val.x, val.y);
        clear_values.push_back(clear_val);
    }

    CXL_LOG(INFO) << "Num frame buffer image views: " << image_views.size();
    vk::FramebufferCreateInfo frame_buffer_info(
        /*flags*/ {},
        /*render_pass*/ render_pass,
        /*image_count*/ image_views.size(),
        /*image_views*/ image_views.data(),
        /*width*/ width,
        /*height*/ height,
        /*layers*/ 1);

    vk::UniqueFramebuffer frame_buffer;
    try {
        frame_buffer = device->vk().createFramebufferUnique(frame_buffer_info);
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    }
    return {.render_pass = render_pass,
            .frame_buffer = std::move(frame_buffer),
            .num_subpasses = static_cast<uint32_t>(subpasses.size()),
            .offset = vk::Offset2D(0, 0),
            .extent = vk::Extent2D(width, height),
            .textures = textures,
            .clear_values = clear_values};
}

void RenderPassBuilder::reset() {
    color_attachments_.clear();
    depth_attachments_.clear();
    resolve_attachments_.clear();
    subpasses_.clear();
    color_textures_.clear();
    depth_textures_.clear();
    resolve_textures_.clear();
    color_clear_values_.clear();
    depth_clear_values_.clear();
}

}  // namespace gfx
