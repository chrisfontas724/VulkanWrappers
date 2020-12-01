// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_ERROR_HANDLER_HPP_
#define GRAPHICS_VK_WRAPPERS_ERROR_HANDLER_HPP_

#include <string>

#include <vulkan/vulkan.hpp>

namespace gfx {
class ErrorHandler {
   public:
    static std::string description(const VkResult& result);
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_ERROR_HANDLER_HPP_