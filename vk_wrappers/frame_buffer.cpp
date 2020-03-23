// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/frame_buffer.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "vk_wrappers/utils/image_utils.hpp"
#include "vk_wrappers/compute_texture.hpp"
#include "logging/logging.hpp"

namespace gfx {

FrameBuffer::FrameBuffer(gfx::LogicalDevicePtr & device,
                         const std::map<AttachmentFlags, ComputeTexturePtr> views,
                         const vk::RenderPass& render_pass,
                         uint32_t width, uint32_t height)
: views_(views)
, width_(width)
, height_(height) {

    try {
        std::vector<vk::ImageView> image_views;
        for (auto& iter : views_) {
            image_views.push_back(iter.second->image_view());
        }

        vk::FramebufferCreateInfo info({}, render_pass, image_views.size(), image_views.data(), width, height, 1);   
        frame_buffer_ = device->vk().createFramebufferUnique(info);
    } catch(vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    } catch(...) {
        std::cout << "unknown error\n";
        exit(-1);
    }
}

FrameBuffer::FrameBuffer(const std::shared_ptr<LogicalDevice> device,
                         const vk::RenderPass& render_pass,
                         AttachmentFlags flags,
                         uint32_t width, uint32_t height) 
: width_(width)
, height_(height) {
    try {
        if ((flags & AttachmentFlags::kColor1) != AttachmentFlags::kNoAttachments) {
            auto attachment_data = ImageUtils::createColorAttachment(device, width, height);
            views_[kColor1] = std::make_shared<ComputeTexture>(device, attachment_data);

            CXL_VLOG(3) << "Made color attachment!";
        }
        if ((flags & AttachmentFlags::kColor2) != AttachmentFlags::kNoAttachments) {
            // TODO
        }
        if ((flags & AttachmentFlags::kColor3) != AttachmentFlags::kNoAttachments) {
            // TODO
        }
        if ((flags & AttachmentFlags::kDepth) != AttachmentFlags::kNoAttachments) {
            // TODO
        }
        if ((flags & AttachmentFlags::kAccum) != AttachmentFlags::kNoAttachments) {
            auto attachment_data = ImageUtils::createAccumulationAttachment(device, width, height);
            views_[kAccum] = std::make_shared<ComputeTexture>(device, attachment_data);
        }

        std::vector<vk::ImageView> image_views;
        for (auto& iter : views_) {
            image_views.push_back(iter.second->image_view());
        }

        vk::FramebufferCreateInfo info({}, render_pass, image_views.size(), image_views.data(), width, height, 1);   
        frame_buffer_ = device->vk().createFramebufferUnique(info);
        CXL_VLOG(3) << "Made frame buffer!";
    } catch(vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    } catch(...) {
        std::cout << "unknown error\n";
        exit(-1);
    }
}

FrameBuffer::~FrameBuffer() {
    for (auto& view : views_) {
        view.second.reset();
    }
}

} // gfx