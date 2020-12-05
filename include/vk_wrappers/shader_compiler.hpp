// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_SHADERS_SHADER_COMPILER_HPP_
#define GRAPHICS_SHADERS_SHADER_COMPILER_HPP_

#include <glslang/Public/ShaderLang.h>

#include <streaming/file_system.hpp>

namespace gfx {
class ShaderCompiler {
   public:
    bool compile(const cxl::FileSystem* fs, const std::string& file,
                 const std::vector<std::string>& include_paths,
                 const std::vector<std::string>& macros, std::vector<uint32_t>* output) const;

    bool compile(const EShLanguage shader_type, const std::string& source_code,
                 const std::vector<std::string>& include_paths,
                 const std::vector<std::string>& macros, std::vector<uint32_t>* output) const;
};
}  // namespace gfx

#endif  // GRAPHICS_SHADERS_SHADER_COMPILER_HPP_