// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_ERROR_HANDLER_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_ERROR_HANDLER_HPP_

#include <string>

#include "vk_wrappers/vulkan.hpp"

namespace gfx {
class ErrorHandler {
   public:
    static std::string description(const VkResult& result);
};
}  // namespace gfx

#endif  // DALI_GRAPHICS_VK_WRAPPERS_ERROR_HANDLER_HPP_