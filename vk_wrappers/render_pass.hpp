// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {

// Class that wraps both a vk::RenderPass and vk::Framebuffer.
// Callers can simply pass this into the command buffer with
// beginRenderPass() and it will handle the issue of resolving
// it to a frame buffer.
class RenderPass {
   public:
    // Checks to see if the current render pass is valid. It will
    // fail if certain criteria are not met. For example, an
    // attachment cannot have both |load| and |clear| operations.
    bool validate() const;
};

}  // namespace gfx