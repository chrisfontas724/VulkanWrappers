// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/shader_module.hpp"

#include "logging/logging.hpp"
#include "vk_wrappers/utils/shader_compiler.hpp"

namespace gfx {

ShaderModule::~ShaderModule() { shader_module_.reset(); }

ShaderModule::ShaderModule(const std::shared_ptr<LogicalDevice>& device,
                           const vk::ShaderStageFlagBits& stage, const SpirV& spir_v)
    : device_(device), stage_(stage), spir_v_(spir_v) {
    try {
        vk::ShaderModuleCreateInfo create_info({}, spir_v_.size() * sizeof(uint32_t),
                                               spir_v_.data());
        shader_module_ = device->vk().createShaderModuleUnique(create_info);

        create_info_ = vk::PipelineShaderStageCreateInfo({}, stage_, shader_module_.get(), "main");

    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
    } catch (...) {
        std::cout << "unknown error\n";
    }
}

}  // namespace gfx