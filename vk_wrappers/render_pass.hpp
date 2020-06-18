// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_RENDER_PASS
#define GRAPHICS_VK_WRAPPERS_RENDER_PASS

#include <optional>
#include <vector>

#include "vk_wrappers/compute_texture.hpp"
#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {

// Combined struct representing combined render pass and
// frame buffer data needed to begin a render pass. Clients
// should not construct this class directly and instead should
// build one through the RenderPassBuilder class below.
struct RenderPassInfo {
    vk::RenderPass render_pass;
    vk::UniqueFramebuffer frame_buffer;
    uint32_t num_subpasses;
    vk::Offset2D offset;
    vk::Extent2D extent;
    std::vector<const ComputeTexture*> textures;
};

class RenderPassBuilder {
   public:
    struct AttachmentInfo {
        vk::AttachmentLoadOp load_op;
        vk::AttachmentStoreOp store_op;
        vk::SampleCountFlagBits samples;
    };

    struct SubpassInfo {
        vk::PipelineBindPoint bind_point;
        std::vector<uint32_t> input_indices;
        std::vector<uint32_t> color_indices;
        std::optional<uint32_t> resolve_index;
        std::optional<uint32_t> depth_index;
    };

    RenderPassBuilder(LogicalDevicePtr device);

    void addColorAttachment(ComputeTexturePtr texture,
                            AttachmentInfo info = kDefaultColorAttachment);

    void addResolveAttachment(ComputeTexturePtr texture, AttachmentInfo info);

    void addDepthAttachment(ComputeTexturePtr texture,
                            AttachmentInfo info = kDefaultDepthAttachment);

    void addSubpass(SubpassInfo);

    void reset();

    RenderPassInfo build();

    static constexpr AttachmentInfo kDefaultColorAttachment = {
        .load_op = vk::AttachmentLoadOp::eClear,
        .store_op = vk::AttachmentStoreOp::eStore,
        .samples = vk::SampleCountFlagBits::e1,
    };

    static constexpr AttachmentInfo kDefaultDepthAttachment = {
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

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_RENDER_PASS