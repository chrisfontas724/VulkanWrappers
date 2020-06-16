// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_SHADERS_SHADER_MODULE_HPP_
#define GRAPHICS_SHADERS_SHADER_MODULE_HPP_

#include "streaming/file_stream.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "vk_wrappers/utils/shader_compiler.hpp"

namespace gfx {

// Wrapper around vk::ShaderModule.
class ShaderModule {
   public:
    ShaderModule(const std::shared_ptr<LogicalDevice>& device,
                 const ShaderCompiler& compiler,
                 const cxl::FileSystem* fs,
                 const std::string& file);

    ShaderModule(const std::shared_ptr<LogicalDevice>& device, vk::ShaderStageFlagBits stage,
                 std::vector<uint32_t> spir_v);

    ~ShaderModule();

    const vk::ShaderModule& vk() const { return shader_module_.get(); }

    const vk::PipelineShaderStageCreateInfo& create_info() const { return create_info_; }

    const vk::ShaderStageFlagBits stage() const { return stage_; }

    const std::vector<uint32_t>& spir_v() const { return spir_v_; }

   private:
    std::weak_ptr<LogicalDevice> device_;
    vk::UniqueShaderModule shader_module_;
    vk::PipelineShaderStageCreateInfo create_info_;
    vk::ShaderStageFlagBits stage_;
    std::vector<uint32_t> spir_v_;
};
}  // namespace gfx

#endif  // GRAPHICS_SHADERS_SHADER_MODULE_HPP_