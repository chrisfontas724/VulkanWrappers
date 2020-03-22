// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_COMPUTE_PIPELINE_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_COMPUTE_PIPELINE_HPP_

#include "vk_wrappers/pipeline.hpp"
#include "logical_device.hpp"
#include "vk_wrappers/utils/shader_compiler.hpp"
#include "vk_wrappers/utils/reflection/reflection.hpp"
#include "vk_wrappers/descriptor_allocator.hpp"
#include "vk_wrappers/shader_module.hpp"
#include "streaming/file_system.hpp"

namespace gfx {
class ComputePipeline : public Pipeline {
public:
    ComputePipeline(std::shared_ptr<LogicalDevice> device,
                    const cxl::FileSystem* fs,
                    const std::string& file);

    ComputePipeline(std::shared_ptr<LogicalDevice> device,
                    const std::vector<uint32_t>& shader);

    ~ComputePipeline();

    vk::PipelineBindPoint bind_point() override { return vk::PipelineBindPoint::eCompute; }

private:

    void __initialize(const std::vector<uint32_t>& spir_v);

    std::unique_ptr<Reflection> reflection_;
    std::unique_ptr<ShaderModule> shader_;
};
}

#endif // DALI_GRAPHICS_VK_WRAPPERS_COMPUTE_PIPELINE_HPP_

