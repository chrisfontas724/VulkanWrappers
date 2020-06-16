// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/forward_declarations.hpp"
#include "vk_wrappers/compute_texture.hpp"

namespace gfx {

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