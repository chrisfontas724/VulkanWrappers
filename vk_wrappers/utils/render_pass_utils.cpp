// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "render_pass_utils.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "core/logging/logging.hpp"

namespace gfx {

vk::RenderPass RenderPassUtils::createStandardRenderPass(LogicalDevicePtr device, bool should_clear) {
    CXL_DCHECK(device);
    
    vk::AttachmentDescription colorAttachment({}, 
        // A standard render pass simply renders into a traditional
        // 32-bit total (8 bits per channel) RGBA buffer, but with
        // the ordering of channels flipped, so it is BGRA.
        vk::Format::eB8G8R8A8Unorm,
        vk::SampleCountFlagBits::e1,
        // If clear is true, the render pass will clear its attachment framebuffer at
        // the start of each render pass, else it will keep the previous pixel
        // values and render on top of them.
        should_clear ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal
    );
    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef);
    vk::RenderPassCreateInfo pass_info({}, 1, &colorAttachment, 1, &subpass);
    return device->vk().createRenderPass(pass_info);
}

vk::RenderPass RenderPassUtils::createAccumulationRenderPass(LogicalDevicePtr device) {
    CXL_DCHECK(device);
    
    vk::AttachmentDescription colorAttachment({}, 
        // An accumulation pass needs to have high precision, as it will potentially be storing
        // samples from hundreds or thousands of iterations, and so we set its format
        // to be RGBA floating point.
        vk::Format::eR32G32B32A32Sfloat,
        vk::SampleCountFlagBits::e1,
        // Making this value |eLoad| ensures that the render pass will not clear its
        // framebuffer's contents at the beginning of a pass, which will make it
        // act as an accumulation buffer.
        vk::AttachmentLoadOp::eLoad,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eColorAttachmentOptimal
    );
    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef);
    vk::RenderPassCreateInfo pass_info({}, 1, &colorAttachment, 1, &subpass);
    return device->vk().createRenderPass(pass_info);
}

vk::RenderPass RenderPassUtils::createPresentationRenderPass(LogicalDevicePtr device, vk::Format format) {
    CXL_DCHECK(device);
    vk::AttachmentDescription presentAttachment({},
        // The optimal format for presentation must be passed in from the user, as it is
        // a value that gets queried by the swapchain.
        format,
        vk::SampleCountFlagBits::e1,
        // Presentation render pass should clear its attached framebuffer after each pass.
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        // Presentation render passes need to be able to present to the screen,
        // so the image layout must be ePresentSrcKHR.
        vk::ImageLayout::ePresentSrcKHR
    );

    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eSharedPresentKHR);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef);
    vk::RenderPassCreateInfo display_pass_info({}, 1, &presentAttachment, 1, &subpass);
    return device->vk().createRenderPass(display_pass_info);
}

} // gfx