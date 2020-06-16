// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_UTILS_RENDER_PASS_UTILS_HPP_
#define GRAPHICS_VK_WRAPPERS_UTILS_RENDER_PASS_UTILS_HPP_

#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {
class RenderPassUtils {
   public:
    static vk::RenderPass createStandardRenderPass(LogicalDevicePtr device,
                                                   bool should_clear = true);

    static vk::RenderPass createAccumulationRenderPass(LogicalDevicePtr device);
    static vk::RenderPass createPresentationRenderPass(LogicalDevicePtr device, vk::Format format);
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_UTILS_RENDER_PASS_UTILS_HPP_