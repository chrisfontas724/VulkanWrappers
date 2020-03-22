// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_SHADERS_SHADER_COMPILER_HPP_
#define DALI_GRAPHICS_SHADERS_SHADER_COMPILER_HPP_

#include "streaming/file_system.hpp"
#include <glslang/Public/ShaderLang.h>

namespace gfx {
class ShaderCompiler {
public:

    ShaderCompiler(std::vector<std::string> macros = {})
    : macros_(macros){}

    bool compile(const cxl::FileSystem* fs,
                 const std::string& file,
                 std::vector<uint32_t>* output);

    bool compile(const EShLanguage shader_type,
                 const std::string& source_code, 
                 std::vector<uint32_t>* output);

private:

    std::vector<std::string> macros_;
};
} // gfx


#endif // DALI_GRAPHICS_SHADERS_SHADER_COMPILER_HPP_