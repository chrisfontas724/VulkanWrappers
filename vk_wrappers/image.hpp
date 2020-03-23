// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef VK_WRAPPERS_IMAGE_HPP_
#define VK_WRAPPERS_IMAGE_HPP_

#include "vk_wrappers/forward_declarations.hpp"

namespace gfx {

// Wrapper for vk::Image.
class Image {
   public:
    struct Info {
        vk::Format format = vk::Format::eUndefined;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t sample_count = 1;
        vk::ImageUsageFlags usage;
        vk::MemoryPropertyFlags memory_flags = vk::MemoryPropertyFlagBits::eDeviceLocal;
        vk::ImageTiling tiling = vk::ImageTiling::eOptimal;
        bool is_mutable = true;
        bool is_external = false;

        bool operator==(const Info& other) const;
    };

    const vk::Image& vk() const { return image_; }

    uint32_t width() const { return info_.width; }
    uint32_t height() const;

    LogicalDevicePtr device() const { return device_.lock(); }

    // Used for transitioning image layouts with pipeline barriers.
    static std::pair<vk::PipelineStageFlags, vk::AccessFlags> stageAndAccessFlagsForLayout(
        const vk::ImageLayout& layout);

   private:
    LogicalDevicePtr device_;
    vk::Image image_;
    Info info_;
};

}  // namespace gfx

#endif  // VK_WRAPPERS_IMAGE_HPP_