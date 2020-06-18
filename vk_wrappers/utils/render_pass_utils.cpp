// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "render_pass_utils.hpp"

#include "logging/logging.hpp"
#include "vk_wrappers/logical_device.hpp"

namespace gfx {

// A standard render pass simply renders into a traditional
// 32-bit total (8 bits per channel) RGBA buffer, but with
// the ordering of channels flipped, so it is BGRA.
// If clear is true, the render pass will clear its attachment framebuffer at
// the start of each render pass, else it will keep the previous pixel
// values and render on top of them.
vk::RenderPass RenderPassUtils::createStandardRenderPass(LogicalDevicePtr device,
                                                         bool should_clear) {
    CXL_DCHECK(device);
    vk::AttachmentDescription colorAttachment(
        /*flags*/{},
        /*format*/vk::Format::eB8G8R8A8Unorm, 
        /*samples*/vk::SampleCountFlagBits::e1,
        /*loadOp*/should_clear ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad,
        /*storeOp*/vk::AttachmentStoreOp::eStore, 
        /*stencilLoadOp*/vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/vk::ImageLayout::eUndefined,
        /*finalLayout*/vk::ImageLayout::eColorAttachmentOptimal);

    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1,
                                   &colorAttachmentRef);

    vk::RenderPassCreateInfo pass_info({}, 1, &colorAttachment, 1, &subpass);
    return device->vk().createRenderPass(pass_info);
}

// An accumulation pass needs to have high precision, as it will potentially be storing
// samples from hundreds or thousands of iterations, and so we set its format
// to be RGBA floating point.
// Making this value |eLoad| ensures that the render pass will not clear its
// framebuffer's contents at the beginning of a pass, which will make it
// act as an accumulation buffer.
vk::RenderPass RenderPassUtils::createAccumulationRenderPass(LogicalDevicePtr device) {
    CXL_DCHECK(device);
    vk::AttachmentDescription colorAttachment(
        /*flags*/{},
        /*format*/vk::Format::eR32G32B32A32Sfloat,
        /*samples*/vk::SampleCountFlagBits::e1,
        /*loadOp*/vk::AttachmentLoadOp::eLoad,
        /*storeOp*/vk::AttachmentStoreOp::eStore,
        /*stencilLoadOp*/vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/vk::ImageLayout::eUndefined,
        /*finalLayout*/vk::ImageLayout::eColorAttachmentOptimal);
    
    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1,
                                   &colorAttachmentRef);
    vk::RenderPassCreateInfo pass_info({}, 1, &colorAttachment, 1, &subpass);
    return device->vk().createRenderPass(pass_info);
}


// The optimal format for presentation must be passed in from the user, as it is
// a value that gets queried by the swapchain.
// Presentation render pass should clear its attached framebuffer after each pass.
// Presentation render passes need to be able to present to the screen,
// so the image layout must be ePresentSrcKHR.
vk::RenderPass RenderPassUtils::createPresentationRenderPass(LogicalDevicePtr device,
                                                             vk::Format format) {
    CXL_DCHECK(device);
    vk::AttachmentDescription presentAttachment(
        /*flags*/{},
        /*format*/format,
        /*samples*/vk::SampleCountFlagBits::e1,
        /*loadOp*/vk::AttachmentLoadOp::eClear,
        /*storeOp*/vk::AttachmentStoreOp::eStore,
        /*stencilLoadOp*/vk::AttachmentLoadOp::eDontCare,
        /*stencilStoreOp*/vk::AttachmentStoreOp::eDontCare,
        /*initialLayout*/vk::ImageLayout::eUndefined,
        /*finalLayout*/vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eSharedPresentKHR);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1,
                                   &colorAttachmentRef);
    vk::RenderPassCreateInfo display_pass_info({}, 1, &presentAttachment, 1, &subpass);
    return device->vk().createRenderPass(display_pass_info);
}

}  // namespace gfxall_attachments.data(),