// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_COMPUTE_PIPELINE_HPP_
#define GRAPHICS_VK_WRAPPERS_COMPUTE_PIPELINE_HPP_

#include "logical_device.hpp"
#include "streaming/file_system.hpp"
#include "vk_wrappers/descriptor_allocator.hpp"
#include "vk_wrappers/pipeline.hpp"
#include "vk_wrappers/shader_module.hpp"
#include "vk_wrappers/utils/reflection/reflection.hpp"
#include "vk_wrappers/utils/shader_compiler.hpp"

namespace gfx {
class ComputePipeline : public Pipeline {
   public:
    ComputePipeline(std::shared_ptr<LogicalDevice> device, const cxl::FileSystem* fs,
                    const std::string& file);

    ComputePipeline(std::shared_ptr<LogicalDevice> device, const std::vector<uint32_t>& shader);

    ~ComputePipeline();

    vk::PipelineBindPoint bind_point() override { return vk::PipelineBindPoint::eCompute; }

   private:
    void __initialize(const std::vector<uint32_t>& spir_v);

    std::unique_ptr<Reflection> reflection_;
    std::unique_ptr<ShaderModule> shader_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMPUTE_PIPELINE_HPP_
