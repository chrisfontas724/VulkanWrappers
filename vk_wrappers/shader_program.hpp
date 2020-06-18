// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
#define GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_

#include "vk_wrappers/utils/reflection.hpp"
#include "forward_declarations.hpp"

namespace gfx {

//
class ShaderProgram {
public:
    static ShaderProgramPtr createGraphics();

    static ShaderProgramPtr createCompute();

private:
    LogicalDeviceWeakPtr device_;
    std::unique_ptr<Reflection> reflection_;
    vk::PipelineLayout pipeline_layout_;
    std::vector<std::shared_ptr<DescriptorSetLayout>> layouts_;
    std::vector<vk::PushConstantRange> push_constants_;
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
