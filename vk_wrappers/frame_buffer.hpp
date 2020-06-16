// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_FRAME_BUFFER_HPP_
#define GRAPHICS_VK_WRAPPERS_FRAME_BUFFER_HPP_

#include <map>

#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {
class FrameBuffer {
   public:
    enum AttachmentFlags {
        kNoAttachments = 0,
        kColor1 = 0x01,
        kColor2 = 0x01 << 1,
        kColor3 = 0x01 << 2,
        kDepth = 0x01 << 3,
        kAccum = 0x01 << 4,
    };

    FrameBuffer(gfx::LogicalDevicePtr& device,
                const std::map<AttachmentFlags, ComputeTexturePtr> views,
                const vk::RenderPass& render_pass, uint32_t width, uint32_t height);

    // This constructor generates the image views for the frame
    // buffer depending on the attachment flag settings.
    FrameBuffer(gfx::LogicalDevicePtr device, const vk::RenderPass& render_pass,
                AttachmentFlags flags, uint32_t width, uint32_t height);

    ~FrameBuffer();

    const vk::Framebuffer& vk() const { return frame_buffer_.get(); }

    const ComputeTexturePtr view(AttachmentFlags flag) const {
        return views_.count(flag) ? views_.at(flag) : nullptr;
    }

    uint32_t width() const { return width_; }
    uint32_t height() const { return height_; }

    vk::RenderPass render_pass() const { return render_pass_; }

   private:
    uint32_t width_, height_;
    vk::UniqueFramebuffer frame_buffer_;
    std::map<AttachmentFlags, ComputeTexturePtr> views_;
    vk::RenderPass render_pass_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_FRAME_BUFFER_HPP_