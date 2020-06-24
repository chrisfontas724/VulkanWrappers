// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_SHADER_PIPELINE_HPP_
#define GRAPHICS_VK_WRAPPERS_SHADER_PIPELINE_HPP_

#include "vk_wrappers/forward_declarations.hpp"
#include "vk_wrappers/shader_module.hpp"
#include "vk_wrappers/utils/reflection/reflection.hpp"

namespace gfx {

class ShaderPipeline {
   public:
    uint32_t descriptor_set_mask() const { return descriptor_set_mask_; }

   private:
    vk::DescriptorSetLayoutCreateInfo descriptor_layouts[32];
    uint32_t descriptor_set_mask_ = 0u;
};

}  // namespace gfx