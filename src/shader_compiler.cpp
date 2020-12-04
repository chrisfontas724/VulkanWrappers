#include "shader_compiler.hpp"

#include <OGLCompilersDLL/InitializeDll.h>
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <glslang/Include/InitializeGlobals.h>
#include <glslang/MachineIndependent/reflection.h>
#include <glslang/Public/ShaderLang.h>

#include <core/logging.hpp>
#include <core/string_utils.hpp>

namespace {
struct Initializer {
    Initializer() { glslang::InitializeProcess(); }
};
static Initializer initializer;

static TBuiltInResource initResources() {
    TBuiltInResource resources;

    resources.maxLights                                 = 32;
    resources.maxClipPlanes                             = 6;
    resources.maxTextureUnits                           = 32;
    resources.maxTextureCoords                          = 32;
    resources.maxVertexAttribs                          = 64;
    resources.maxVertexUniformComponents                = 4096;
    resources.maxVaryingFloats                          = 64;
    resources.maxVertexTextureImageUnits                = 32;
    resources.maxCombinedTextureImageUnits              = 80;
    resources.maxTextureImageUnits                      = 32;
    resources.maxFragmentUniformComponents              = 4096;
    resources.maxDrawBuffers                            = 32;
    resources.maxVertexUniformVectors                   = 128;
    resources.maxVaryingVectors                         = 8;
    resources.maxFragmentUniformVectors                 = 16;
    resources.maxVertexOutputVectors                    = 16;
    resources.maxFragmentInputVectors                   = 15;
    resources.minProgramTexelOffset                     = -8;
    resources.maxProgramTexelOffset                     = 7;
    resources.maxClipDistances                          = 8;
    resources.maxComputeWorkGroupCountX                 = 65535;
    resources.maxComputeWorkGroupCountY                 = 65535;
    resources.maxComputeWorkGroupCountZ                 = 65535;
    resources.maxComputeWorkGroupSizeX                  = 1024;
    resources.maxComputeWorkGroupSizeY                  = 1024;
    resources.maxComputeWorkGroupSizeZ                  = 64;
    resources.maxComputeUniformComponents               = 1024;
    resources.maxComputeTextureImageUnits               = 16;
    resources.maxComputeImageUniforms                   = 8;
    resources.maxComputeAtomicCounters                  = 8;
    resources.maxComputeAtomicCounterBuffers            = 1;
    resources.maxVaryingComponents                      = 60;
    resources.maxVertexOutputComponents                 = 64;
    resources.maxGeometryInputComponents                = 64;
    resources.maxGeometryOutputComponents               = 128;
    resources.maxFragmentInputComponents                = 128;
    resources.maxImageUnits                             = 8;
    resources.maxCombinedImageUnitsAndFragmentOutputs   = 8;
    resources.maxCombinedShaderOutputResources          = 8;
    resources.maxImageSamples                           = 0;
    resources.maxVertexImageUniforms                    = 0;
    resources.maxTessControlImageUniforms               = 0;
    resources.maxTessEvaluationImageUniforms            = 0;
    resources.maxGeometryImageUniforms                  = 0;
    resources.maxFragmentImageUniforms                  = 8;
    resources.maxCombinedImageUniforms                  = 8;
    resources.maxGeometryTextureImageUnits              = 16;
    resources.maxGeometryOutputVertices                 = 256;
    resources.maxGeometryTotalOutputComponents          = 1024;
    resources.maxGeometryUniformComponents              = 1024;
    resources.maxGeometryVaryingComponents              = 64;
    resources.maxTessControlInputComponents             = 128;
    resources.maxTessControlOutputComponents            = 128;
    resources.maxTessControlTextureImageUnits           = 16;
    resources.maxTessControlUniformComponents           = 1024;
    resources.maxTessControlTotalOutputComponents       = 4096;
    resources.maxTessEvaluationInputComponents          = 128;
    resources.maxTessEvaluationOutputComponents         = 128;
    resources.maxTessEvaluationTextureImageUnits        = 16;
    resources.maxTessEvaluationUniformComponents        = 1024;
    resources.maxTessPatchComponents                    = 120;
    resources.maxPatchVertices                          = 32;
    resources.maxTessGenLevel                           = 64;
    resources.maxViewports                              = 16;
    resources.maxVertexAtomicCounters                   = 0;
    resources.maxTessControlAtomicCounters              = 0;
    resources.maxTessEvaluationAtomicCounters           = 0;
    resources.maxGeometryAtomicCounters                 = 0;
    resources.maxFragmentAtomicCounters                 = 8;
    resources.maxCombinedAtomicCounters                 = 8;
    resources.maxAtomicCounterBindings                  = 1;
    resources.maxVertexAtomicCounterBuffers             = 0;
    resources.maxTessControlAtomicCounterBuffers        = 0;
    resources.maxTessEvaluationAtomicCounterBuffers     = 0;
    resources.maxGeometryAtomicCounterBuffers           = 0;
    resources.maxFragmentAtomicCounterBuffers           = 1;
    resources.maxCombinedAtomicCounterBuffers           = 1;
    resources.maxAtomicCounterBufferSize                = 16384;
    resources.maxTransformFeedbackBuffers               = 4;
    resources.maxTransformFeedbackInterleavedComponents = 64;
    resources.maxCullDistances                          = 8;
    resources.maxCombinedClipAndCullDistances           = 8;
    resources.maxSamples                                = 4;
    resources.maxMeshOutputVerticesNV                   = 256;
    resources.maxMeshOutputPrimitivesNV                 = 512;
    resources.maxMeshWorkGroupSizeX_NV                  = 32;
    resources.maxMeshWorkGroupSizeY_NV                  = 1;
    resources.maxMeshWorkGroupSizeZ_NV                  = 1;
    resources.maxTaskWorkGroupSizeX_NV                  = 32;
    resources.maxTaskWorkGroupSizeY_NV                  = 1;
    resources.maxTaskWorkGroupSizeZ_NV                  = 1;
    resources.maxMeshViewCountNV                        = 4;

    resources.limits.nonInductiveForLoops                 = 1;
    resources.limits.whileLoops                           = 1;
    resources.limits.doWhileLoops                         = 1;
    resources.limits.generalUniformIndexing               = 1;
    resources.limits.generalAttributeMatrixVectorIndexing = 1;
    resources.limits.generalVaryingIndexing               = 1;
    resources.limits.generalSamplerIndexing               = 1;
    resources.limits.generalVariableIndexing              = 1;
    resources.limits.generalConstantMatrixVectorIndexing  = 1;

    return resources;
}

}  // namespace

namespace gfx {

// Determine the shader stage based off of the extension
// used on the file.
EShLanguage getShaderStage(const std::string& stage) {
    if (stage == "vert") {
        return EShLangVertex;
    } else if (stage == "tesc") {
        return EShLangTessControl;
    } else if (stage == "tese") {
        return EShLangTessEvaluation;
    } else if (stage == "geom") {
        return EShLangGeometry;
    } else if (stage == "frag") {
        return EShLangFragment;
    } else if (stage == "comp") {
        return EShLangCompute;
    } else if (stage == "raygen") {
        return EShLangRayGenNV;
    } else {
        CXL_CHECK(0) << "Unknown shader stage";
        return EShLangCount;
    }
}

bool ShaderCompiler::compile(const EShLanguage shader_type, const std::string& source_code,
                             const std::vector<std::string>& include_paths,
                             const std::vector<std::string>& macros,
                             std::vector<uint32_t>* output) const {
    // Constants.
    int32_t ClientInputSemanticsVersion = 110;  // maps to, say, #define VULKAN 100
    glslang::EShTargetClientVersion VulkanClientVersion = glslang::EShTargetVulkan_1_1;
    glslang::EShTargetLanguageVersion TargetVersion = glslang::EShTargetSpv_1_0;
    const int DefaultVersion = 450;

    // Clear the output.
    output->clear();

    // Include directory information.
    DirStackFileIncluder includer;

    // Add include header paths.
    for (const auto& include_path : include_paths) {
        includer.pushExternalLocalDirectory(include_path);
    }

    glslang::TShader shader(shader_type);

    shader.setEnvInput(glslang::EShSourceGlsl, shader_type, glslang::EShClientVulkan,
                       ClientInputSemanticsVersion);
    shader.setEnvClient(glslang::EShClientVulkan, VulkanClientVersion);
    shader.setEnvTarget(glslang::EShTargetSpv, TargetVersion);

    // Extend the shader with version number and google include directive.
    std::string extended_text = "#version " + std::to_string(DefaultVersion) + "\n";
    extended_text += "#extension GL_GOOGLE_include_directive : enable\n";
    extended_text += "#extension GL_ARB_separate_shader_objects : enable\n";

    // Add user defined macros.
    for (auto& macro : macros) {
        extended_text += "#define " + macro + "\n";
    }

    extended_text += source_code;
    const char* final_code = extended_text.c_str();
    shader.setStrings(&final_code, 1);

    // Preprocessing
    TBuiltInResource resources = initResources();
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

    std::string PreprocessedGLSL;
    if (!shader.preprocess(&resources, DefaultVersion, ENoProfile, false, false, messages,
                           &PreprocessedGLSL, includer)) {
        CXL_LOG(INFO) << "GLSL Preprocessing Failed." << std::endl;
        CXL_LOG(INFO) << shader.getInfoLog() << std::endl;
        CXL_LOG(INFO) << shader.getInfoDebugLog() << std::endl;
        return false;
    }

    const char* preprocessedCStr = PreprocessedGLSL.c_str();
    shader.setStrings(&preprocessedCStr, 1);

    // Compile.
    if (!shader.parse(&resources, DefaultVersion, true, messages)) {
        CXL_LOG(INFO) << "GLSL Parsing Failed.";
        CXL_LOG(INFO) << shader.getInfoLog();
        CXL_LOG(INFO) << shader.getInfoDebugLog();
        return false;
    }

    glslang::TProgram program;
    program.addShader(&shader);

    // Link.
    if (!program.link(messages)) {
        CXL_LOG(INFO) << "GLSL Linking Failed";
        CXL_LOG(INFO) << shader.getInfoLog();
        CXL_LOG(INFO) << shader.getInfoDebugLog();
        return false;
    }

    // Convert to SPIRV.
    spv::SpvBuildLogger logger;
    glslang::SpvOptions spvOptions;
    glslang::GlslangToSpv(*program.getIntermediate(shader_type), *output, &logger, &spvOptions);

    return true;
}

bool ShaderCompiler::compile(const cxl::FileSystem* fs, const std::string& file,
                             const std::vector<std::string>& include_paths,
                             const std::vector<std::string>& macros,
                             std::vector<uint32_t>* output) const {
    std::string extension;
    if (!fs->fileExists(file)) {
        CXL_LOG(WARNING) << "Shader file does not exist!";
        return false;
    }

    if (!cxl::FileSystem::getExtension(file, extension)) {
        CXL_LOG(WARNING) << "Shader file does not have an extension";
        return false;
    }

    std::string text;
    fs->readFile(file, text);
    if (text.size() <= 0) {
        return false;
    }

    EShLanguage shader_type = getShaderStage(extension);
    return compile(shader_type, text, include_paths, macros, output);
}

}  // namespace gfx
