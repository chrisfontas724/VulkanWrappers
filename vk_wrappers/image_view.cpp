// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/image_view.hpp"

namespace gfx {

ImageViewPtr ImageView::create(const ImagePtr& image, vk::ImageAspectFlags flags) {
    if (!image->device()) {
        return nullptr;
    }
    return std::shared_ptr<ImageView>(new ImageView(image, flags));
}


ImageView::ImageView(const ImagePtr& image, vk::ImageAspectFlags flags)
: image_(image) {
    const auto& info = image->info();
    auto device = image->device();
    vk::ImageSubresourceRange range(flags,0,1,0,1);
    vk::ImageViewCreateInfo info(
        /*ImageViewCreateFlags*/{}, 
        image->vk(),
        vk::ImageViewType::e2D,
        format,
        vk::ComponentMapping(), 
        range
    );
    image_view_ = device->vk().createImageView(info);
}

uint32_t ImageView::width() const {
    return image_->width();
}

uint32_t ImageView::height() const {
    return image_->height();
}

} // gfx