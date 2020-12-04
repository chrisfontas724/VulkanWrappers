// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
#define GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_

#include "forward_declarations.hpp"
#include "shader_module.hpp"
#include "shader_pipeline.hpp"
#include "reflection/reflection.hpp"

namespace gfx {

class ShaderProgram {
   public:
    static ShaderProgramPtr createGraphics(const LogicalDevicePtr& device, const SpirV& vertex,
                                           const SpirV& fragment);

    static ShaderProgramPtr createCompute(const LogicalDevicePtr& device, const SpirV& kernel);

    const ShaderPipeline& pipeline() const { return *pipeline_.get(); }

    vk::PipelineBindPoint bind_point() const { return bind_point_; }

    const ShaderModule* module(vk::ShaderStageFlagBits stage) const {
        return shader_modules_.at(stage).get();
    }

   private:
    ShaderProgram(const LogicalDevicePtr& device, const vk::PipelineBindPoint& bind_point,
                  const std::map<vk::ShaderStageFlagBits, SpirV>& spirv);

    LogicalDeviceWeakPtr device_;
    std::unique_ptr<Reflection> reflection_;
    vk::PipelineBindPoint bind_point_;
    ShaderPipelinePtr pipeline_;
    std::map<vk::ShaderStageFlagBits, std::unique_ptr<ShaderModule>> shader_modules_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
