// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_UTILS_IMAGE_UTILS_HPP_
#define GRAPHICS_VK_WRAPPERS_UTILS_IMAGE_UTILS_HPP_

#include <IlmBase/Half/half.h>

#include "vk_wrappers/logical_device.hpp"

namespace gfx {
class ImageUtils {
   public:
    struct Data {
        vk::Image image;
        vk::DeviceMemory memory;
        vk::Format format;
        vk::Extent2D extent;
        vk::ImageLayout layout;
    };

    static vk::ImageView createImageView(std::shared_ptr<LogicalDevice> device, const Data& data);
    static vk::ImageView createImageView(std::shared_ptr<LogicalDevice> device,
                                         const vk::Image& image, const vk::Format& format);

    static Data create8BitUnormImage(std::shared_ptr<LogicalDevice> device, uint32_t width,
                                     uint32_t height, uint32_t channels, const uint8_t* pixels);

    static Data createHDRImage(std::shared_ptr<LogicalDevice> device, uint32_t width,
                               uint32_t height, const half* pixels);
    static Data createColorAttachment(std::shared_ptr<LogicalDevice> device, uint32_t width,
                                      uint32_t height);
    static Data createAccumulationAttachment(std::shared_ptr<LogicalDevice> device, uint32_t width,
                                             uint32_t height);
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_UTILS_IMAGE_UTILS_HPP_