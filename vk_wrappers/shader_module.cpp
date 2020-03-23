// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/shader_module.hpp"

#include "logging/logging.hpp"
#include "vk_wrappers/utils/shader_compiler.hpp"

namespace gfx {

namespace {
vk::ShaderStageFlagBits getStage(const std::string& extension) {
    if (extension == "vert") {
        return vk::ShaderStageFlagBits::eVertex;
    } else if (extension == "frag") {
        return vk::ShaderStageFlagBits::eFragment;
    } else if (extension == "comp") {
        return vk::ShaderStageFlagBits::eCompute;
    } else {
        CXL_DCHECK(false) << "Invalid shader extension type" << extension;
        return vk::ShaderStageFlagBits::eAll;
    }
}
}  // namespace

ShaderModule::~ShaderModule() { shader_module_.reset(); }

ShaderModule::ShaderModule(const std::shared_ptr<LogicalDevice>& device, const cxl::FileSystem* fs,
                           const std::string& file)
    : device_(device) {
    ShaderCompiler compiler;
    if (!compiler.compile(fs, file, &spir_v_)) {
        throw std::runtime_error("Could not create shader.");
    }

    std::string extension;
    if (!cxl::FileSystem::getExtension(file, extension)) {
        CXL_LOG(WARNING) << "Shader file does not have an extension";
    }

    stage_ = getStage(extension);

    try {
        vk::ShaderModuleCreateInfo create_info({}, spir_v_.size() * sizeof(uint32_t),
                                               spir_v_.data());
        shader_module_ = device->vk().createShaderModuleUnique(create_info);

        create_info_ = vk::PipelineShaderStageCreateInfo({}, stage_, vk(), "main");

    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
    } catch (...) {
        std::cout << "unknown error\n";
    }
}

ShaderModule::ShaderModule(const std::shared_ptr<LogicalDevice>& device,
                           vk::ShaderStageFlagBits stage, std::vector<uint32_t> spir_v)
    : device_(device), stage_(stage), spir_v_(spir_v) {
    try {
        vk::ShaderModuleCreateInfo create_info({}, spir_v_.size() * sizeof(uint32_t),
                                               spir_v_.data());
        shader_module_ = device->vk().createShaderModuleUnique(create_info);

        create_info_ = vk::PipelineShaderStageCreateInfo({}, stage_, vk(), "main");

    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
    } catch (...) {
        std::cout << "unknown error\n";
    }
}

}  // namespace gfx