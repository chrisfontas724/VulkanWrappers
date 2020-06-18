// Copyright 2020 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
#define GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_

namespace gfx {

//
class ShaderProgram {
    static ShaderProgramPtr createGraphics();

    static ShaderProgramPtr createCompute();
};

}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_SHADER_PROGRAM_HPP_
