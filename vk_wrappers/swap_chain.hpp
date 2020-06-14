// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_SWAP_CHAIN_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_SWAP_CHAIN_HPP_

#include <tuple>

#include "vk_wrappers/frame_buffer.hpp"

namespace gfx {

// Wrapper around Vulkan's SwapchainKHR.
class SwapChain {
   public:
    using RenderFunction = std::function<std::vector<vk::Semaphore>&(
        FrameBufferPtr, vk::Semaphore&, vk::Fence&, uint32_t, uint32_t)>;

    SwapChain(LogicalDevicePtr logical_device, vk::SurfaceKHR surface, uint32_t width,
              uint32_t height);

    ~SwapChain();

    const vk::SwapchainKHR& vk() const { return swap_chain_.get(); }

    void beginFrame(RenderFunction callback);

    const vk::SurfaceFormatKHR format() const { return surface_format_; }

   private:
    static vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    void createFrameBuffers(const vk::RenderPass& render_pass);
    void present(const std::vector<vk::Semaphore>& semaphores);

    std::weak_ptr<LogicalDevice> logical_device_;
    vk::SurfaceKHR surface_;

    vk::UniqueSwapchainKHR swap_chain_;
    vk::SurfaceFormatKHR surface_format_;
    vk::PresentModeKHR present_mode_;
    vk::Extent2D extent_;

    std::vector<vk::Image> images_;
    std::vector<vk::ImageView> image_views_;

    std::vector<vk::Fence> in_flight_fences_;
    std::vector<vk::Fence> images_in_flight_;
    std::vector<vk::Semaphore> image_available_semaphores_;

    std::vector<FrameBufferPtr> frame_buffers_;

    uint32_t image_index_;
    uint32_t current_frame_;
};
}  // namespace gfx

#endif  // DALI_GRAPHICS_VK_WRAPPERS_SWAP_CHAIN_HPP_