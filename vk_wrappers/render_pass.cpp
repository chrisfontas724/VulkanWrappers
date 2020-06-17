// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/render_pass.hpp"
#include "logging/logging.hpp"

namespace gfx {

RenderPassBuilder::RenderPassBuilder(LogicalDevicePtr device) 
: device_(device) {}

void RenderPassBuilder::addColorAttachment(ComputeTexturePtr texture, AttachmentInfo info) {
    vk::AttachmentDescription attachment(
        /*flags*/{},
        /*format*/texture->format(),
        /*samples*/info.samples,
        /*loadOp*/info.load_op,
        /*storeOp*/info.store_op,
        /*stencilLoadOp*/vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/vk::ImageLayout::eUndefined,
        /*finalLayout*/vk::ImageLayout::eColorAttachmentOptimal
    );

    color_attachments_.push_back(attachment);
    color_textures_.push_back(texture);
}

void RenderPassBuilder::addDepthAttachment(ComputeTexturePtr texture, AttachmentInfo info) {
    vk::AttachmentDescription attachment(
        /*flags*/{},
        /*format*/texture->format(),
        /*samples*/info.samples,
        /*loadOp*/info.load_op,
        /*storeOp*/info.store_op,
        /*stencilLoadOp*/vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/vk::ImageLayout::eUndefined,
        /*finalLayout*/vk::ImageLayout::eDepthStencilAttachmentOptimal);

    depth_attachments_.push_back(attachment);
    depth_textures_.push_back(texture);
}

void RenderPassBuilder::addSubpass(SubpassInfo info) {
    subpasses_.push_back(info);
}

vk::RenderPass RenderPassBuilder::build() {
    auto device = device_.lock();
    CXL_DCHECK(device);

    const uint32_t kNumCol = color_attachments_.size();
    const uint32_t kNumDepth = depth_attachments_.size();

    std::vector<vk::AttachmentDescription> all_attachments = color_attachments_;
    all_attachments.insert(all_attachments.end(), depth_attachments_.begin(), depth_attachments_.end());
    all_attachments.insert(all_attachments.end(), resolve_attachments_.begin(), resolve_attachments_.end());


    std::vector<vk::SubpassDescription> subpasses;
    for (const auto& subpass_info : subpasses_) {
        std::vector<vk::AttachmentReference> input_references;
        for (uint32_t index : subpass_info.input_indices) {
            const auto& attachment = color_attachments_[index];
            input_references.push_back(vk::AttachmentReference(index, attachment.finalLayout));
        }

        std::vector<vk::AttachmentReference> color_references;
        for (uint32_t index : subpass_info.color_indices) {
            const auto& attachment = color_attachments_[index];
            color_references.push_back(vk::AttachmentReference(index, attachment.finalLayout));
        }

        // Depth textures are not required.
        vk::AttachmentReference depth_reference;
        if (subpass_info.depth_index.has_value()) {
            uint32_t index = *subpass_info.depth_index;
            const auto& depth_attachment = depth_attachments_[index];
            depth_reference = vk::AttachmentReference(kNumCol + index, depth_attachment.finalLayout);
        }

        // Resolve textures are not required.
        vk::AttachmentReference resolve_reference;
        if (subpass_info.depth_index.has_value()) {
            uint32_t index = *subpass_info.resolve_index;
            const auto& resolve_attachment = resolve_attachments_[index];
            resolve_reference = vk::AttachmentReference(kNumCol + kNumDepth + index, resolve_attachment.finalLayout);
        }

        vk::SubpassDescription subpass(
            /*flags*/{},
            /*bindPoint*/subpass_info.bind_point,
            /*num input*/input_references.size(),
            /*inputs*/input_references.data(),
            /*num color*/color_references.size(),
            /*color refs*/color_references.data(),
            /*resolve ref*/ subpass_info.resolve_index.has_value() ? &resolve_reference : nullptr,
            /*depth ref*/   subpass_info.depth_index.has_value() ? &depth_reference : nullptr);

        subpasses.push_back(subpass);
    }

    vk::RenderPassCreateInfo render_pass_info(
        /*flags*/            {}, 
        /*attachment_count*/ all_attachments.size(), 
        /*attachment_data*/  all_attachments.data(),
        /*subpass_count*/    subpasses.size(), 
        /*subpass_data*/     subpasses.data());
    return device->vk().createRenderPass(render_pass_info);
}

void RenderPassBuilder::reset() {
    color_attachments_.clear();
    depth_attachments_.clear();
    resolve_attachments_.clear();
}

} // gfx
