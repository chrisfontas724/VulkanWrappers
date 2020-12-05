// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_SHADERS_REFLECTION_REFLECTION_HPP_
#define GRAPHICS_SHADERS_REFLECTION_REFLECTION_HPP_

#include "descriptor_allocator.hpp"
#include <vector>

namespace gfx {
class Reflection {
public:

    Reflection(std::shared_ptr<LogicalDevice> device, const std::vector<std::vector<uint32_t>>& spirv);
    ~Reflection();

    std::vector<vk::PushConstantRange> createPushConstants();
    std::vector<std::shared_ptr<DescriptorSetLayout>> createLayouts();

private:   

    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::weak_ptr<LogicalDevice> device_;
};
} // gfx

#endif // GRAPHICS_SHADERS_REFLECTION_REFLECTION_HPP_