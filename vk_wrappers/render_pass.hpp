// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/forward_declarations.hpp"
#include "vk_wrappers/compute_texture.hpp"
#include <optional>

namespace gfx {

class RenderPassBuilder {
    struct AttachmentInfo{
        vk::AttachmentLoadOp load_op;
        vk::AttachmentStoreOp store_op;
        vk::SampleCountFlagBits samples;
    };

    struct SubpassInfo{
        vk::PipelineBindPoint bind_point;
        std::vector<uint32_t> input_indices;
        std::vector<uint32_t> color_indices;
        std::optional<uint32_t> resolve_index;
        std::optional<uint32_t> depth_index;
    }

    RenderPassBuild(LogicalDevicePtr device);

    void addColorAttachment(ComputeTexturePtr texture, AttachmentInfo info = kDefaultColorAttachment);

    void addDepthAttachment(ComputeTexturePtr texture, AttachmentInfo info = kDefaultDepthAttachment);

    void addSubpass(SubpassInfo);

    vk::RenderPass build();

    void reset();

    static const AttachmentInfo kDefaultColorAttachment = {
        .load_op = vk::AttachmentLoadOp::eClear,
        .store_op = vk::AttachmentStoreOp::eStore,
        .samples = vk::SampleCountFlagBits::e1,
    };

    static const AttachmentInfo kDefaultDepthAttachment = {
        .load_op = vk::AttachmentLoadOp::eClear,
        .store_op = vk::AttachmentStoreOp::eDontCare,
        .samples = vk::SampleCountFlagBits::e1,
    };

private:
    LogicalDeviceWeakPtr device_;

    std::vector<vk::AttachmentDescription> color_attachments_;
    std::vector<vk::AttachmentDescription> depth_attachments_;
    std::vector<vk::AttachmentDescription> resolve_attachments_;

    std::vector<ComputeTexturePtr> color_textures_;
    std::vector<ComputeTexturePtr> depth_textures_;
    std::vector<ComputeTexturePtr> resolve_textures_;

    std::vector<SubpassInfo> subpasses_;
};

// Struct that contains all of the data needed to create a vulkan
// render pass and frame buffer. This way those items do not need
// to be managed by the client directly.
struct RenderPass {
   public:

   

    struct SubPass {

    };

    struct AttachmentInfo {

    };

    void addAttachment(ComputeTexturePtr texture, bool is_swapchain_image, bool clear_begin, bool clear_end) {

    }

    void addSubPass(std::vector<uint32_t> inputs, std::vector<uint32_t> outputs, uint32_t depth);

private:

    std::vector<SubPass> subpasses;


    std::vector<ComputeTexturePtr> attachments;

    // Checks to see if the current render pass is valid. It will
    // fail if certain criteria are not met. For example, an
    // attachment cannot have both |load| and |clear| operations.
    bool validate() const;
};

}  // namespace gfx