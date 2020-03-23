// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/image.hpp"

namespace gfx {

class ImageView {
   public:
    ImageViewPtr create(const Image& image, vk::ImageAspectFlags flags);

    const ImagePtr& image() const { return image_; }
    const vk::ImageView& vk() const { return image_view_; }

    uint32_t width() const;
    uint32_t height() const;

   protected:
    explicit ImageView(const ImagePtr& image, vk::ImageAspectFlags flags);

    ImagePtr image_;
    vk::ImageView image_view_;
};

}  // namespace gfx