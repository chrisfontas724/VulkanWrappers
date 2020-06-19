// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_SHADERS_SHADER_MODULE_HPP_
#define GRAPHICS_SHADERS_SHADER_MODULE_HPP_

#include "streaming/file_stream.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "vk_wrappers/utils/shader_compiler.hpp"

namespace gfx {

using SpirV = std::vector<uint32_t>;

// Wrapper around vk::ShaderModule.
class ShaderModule {
   public:
    ShaderModule(const std::shared_ptr<LogicalDevice>& device, 
                 const vk::ShaderStageFlagBits& stage,
                 const SpirV& spir_v);

    ~ShaderModule();

    vk::PipelineShaderStageCreateInfo pipeline_create_info() const { return create_info_; }

    const std::vector<uint32_t>& spir_v() const { return spir_v_; }

    vk::ShaderStageFlagBits stage() const { return stage_; }

   private:
    std::weak_ptr<LogicalDevice> device_;
    vk::UniqueShaderModule shader_module_;
    vk::PipelineShaderStageCreateInfo create_info_;
    vk::ShaderStageFlagBits stage_;
    std::vector<uint32_t> spir_v_;
};
}  // namespace gfx

#endif  // GRAPHICS_SHADERS_SHADER_MODULE_HPP_