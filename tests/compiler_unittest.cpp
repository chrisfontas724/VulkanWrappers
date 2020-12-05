#include <gtest/gtest.h>
#include "vk_wrappers/utils/shader_compiler.hpp"
#include "vk_wrappers/utils/reflection/spirv_reflect.h"
#include <streaming/file_system.hpp>
#include "logging/logging.hpp"

int SpirvReflectExample(const void* spirv_code, size_t spirv_nbytes) {
    CXL_DCHECK(spirv_code);
    CXL_DCHECK(spirv_nbytes > 0);
    // Generate reflection data for a shader
    SpvReflectShaderModule module;
    SpvReflectResult result = spvReflectCreateShaderModule(spirv_nbytes, spirv_code, &module);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    uint32_t num_sets = module.descriptor_set_count;
    for (uint32_t i = 0; i < num_sets; i++) {
        SpvReflectDescriptorSet set = module.descriptor_sets[i];
        for (uint32_t j = 0; j < set.binding_count; j++) {
            auto& binding = set.bindings[j];
            CXL_DCHECK(binding);
            CXL_LOG(INFO) << "Binding name: " << binding->name;
        }
    }

    // Output variables, descriptor bindings, descriptor sets, and push constants
    // can be enumerated and extracted using a similar mechanism.

    // Destroy the reflection data when no longer required.
   // spvReflectDestroyShaderModule(&module);
    return 0;
}

namespace gfx {

TEST(ShaderCompiler, BadInput) {
    cxl::FileSystem fs;
    ShaderCompiler compiler;
    std::vector<uint32_t> output;
    bool result = compiler.compile(&fs, "test.vert", &output);
    EXPECT_FALSE(result);

    fs = cxl::FileSystem("/Users/Chris/Desktop/NewChristalz/common/shaders");
    result = compiler.compile(&fs, "test", &output);
    EXPECT_FALSE(result);
}

TEST(ShaderCompiler, GoodInput) {
    std::vector<uint32_t> output;
    cxl::FileSystem fs = cxl::FileSystem("/Users/Chris/Desktop/NewChristalz/common/shaders");

    ShaderCompiler compiler;
    bool result = compiler.compile(&fs, "test.vert", &output);
    EXPECT_TRUE(result);
}

TEST(ShaderCompiler, Reflection) {
    std::vector<uint32_t> output;
    cxl::FileSystem fs = cxl::FileSystem("/Users/Chris/Desktop/NewChristalz/common/shaders");

    ShaderCompiler compiler;
    bool result = compiler.compile(&fs, "test.vert", &output);
    EXPECT_TRUE(result);

    SpirvReflectExample(reinterpret_cast<const void*>(output.data()), output.size()*sizeof(uint32_t));


    CXL_LOG(INFO) << "SPIRV SIZE: " << output.size();
    auto module = spv_reflect::ShaderModule(output);

    uint32_t num_descriptor_sets;
    module.EnumerateDescriptorSets(&num_descriptor_sets, nullptr);
    CXL_LOG(INFO) << "Num Descriptor Sets: " << num_descriptor_sets;


    SpvReflectDescriptorSet* descriptor_sets = new SpvReflectDescriptorSet[num_descriptor_sets];
    module.EnumerateDescriptorSets(&num_descriptor_sets, &descriptor_sets);

    // for (uint32_t i = 0; i < num_descriptor_sets; i++) {
    //     auto& descriptor_set = descriptor_sets[i];
    //     CXL_LOG(INFO) << "  Set binding count: " << descriptor_set.binding_count;
    //     CXL_LOG(INFO) << "  Set index: " << descriptor_set.set;

    //     for (uint32_t j = 0; j < descriptor_set.binding_count; j++) {

    //         auto& binding =  descriptor_set.bindings[j];

    //         CXL_DCHECK(binding) << "Binding was null";
    //         CXL_LOG(INFO) << "BINDING INDEX: " << binding->binding;
    //         CXL_LOG(INFO) << "BINDING TYPE: " << binding->descriptor_type;
    //     }
    // }
}

} // gfx