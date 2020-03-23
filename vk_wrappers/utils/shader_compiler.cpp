#include "vk_wrappers/utils/shader_compiler.hpp"
#include <OGLCompilersDLL/InitializeDll.h>
#include <glslang/Include/InitializeGlobals.h>
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include "logging/logging.hpp"
#include "utils/string_utils.hpp"
#include <glslang/MachineIndependent/reflection.h>

namespace {
struct Initializer {
    Initializer() {
        glslang::InitializeProcess();
    }
};
static Initializer initializer;
static const TBuiltInResource kDefaultTBuiltInResource = {

    .maxComputeWorkGroupCountX = 4096,
    .maxComputeWorkGroupCountY = 4096,
    .maxComputeWorkGroupCountZ = 4096,
    .maxComputeWorkGroupSizeX = 4096,
    .maxComputeWorkGroupSizeY = 4096,
    .maxComputeWorkGroupSizeZ = 4096,
    .maxDrawBuffers = 2,
    .maxVertexOutputComponents = 10,
    .maxFragmentInputComponents = 10,
    .limits.generalUniformIndexing = true, // Allow non-const indexing.
    .limits.generalVariableIndexing = true, // Allow non-const indexing.
    .limits.generalSamplerIndexing = true, 
    .limits.generalVaryingIndexing = true,
    .limits.nonInductiveForLoops = true,
    .limits.generalConstantMatrixVectorIndexing = true,
    .limits.whileLoops = true,
    .limits.doWhileLoops = true
 };

} // namespace

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
    } else {
        CXL_CHECK(0) << "Unknown shader stage";
        return EShLangCount;
    }
}

bool ShaderCompiler::compile(const EShLanguage shader_type, 
                            const std::string& source_code,
                            std::vector<uint32_t>* output) {
    // Constants.
    int32_t ClientInputSemanticsVersion = 110; // maps to, say, #define VULKAN 100
    glslang::EShTargetClientVersion VulkanClientVersion = glslang::EShTargetVulkan_1_1;
    glslang::EShTargetLanguageVersion TargetVersion = glslang::EShTargetSpv_1_0;
    const int DefaultVersion = 450;

    // Include directory information.
    DirStackFileIncluder includer;

    // Get Path of File
    // TODO: Remove this hardcoding.
    //std::string path = fs->directory();
    //includer.pushExternalLocalDirectory(path);
    const auto& header_dir = cxl::FileSystem::currentPath() + "../source/shaders/vulkan/header_files/";
    includer.pushExternalLocalDirectory(header_dir);

    glslang::TShader shader(shader_type);

    shader.setEnvInput(glslang::EShSourceGlsl, shader_type, glslang::EShClientVulkan, ClientInputSemanticsVersion);
    shader.setEnvClient(glslang::EShClientVulkan, VulkanClientVersion);
    shader.setEnvTarget(glslang::EShTargetSpv, TargetVersion);

    // Extend the shader with version number and google include directive.
    std::string extended_text = "#version " + std::to_string(DefaultVersion) + "\n";
    extended_text += "#extension GL_GOOGLE_include_directive : enable\n";
    extended_text += "#extension GL_ARB_separate_shader_objects : enable\n";

    // Add user defined macros.
    for (auto& macro : macros_) {
        extended_text += "#define " + macro + "\n";
    }

    extended_text += source_code;
    const char* final_code = extended_text.c_str();
    shader.setStrings(&final_code, 1);

    // Preprocessing 
    TBuiltInResource resources;
    resources = kDefaultTBuiltInResource;
    EShMessages messages = (EShMessages) (EShMsgSpvRules | EShMsgVulkanRules);

    std::string PreprocessedGLSL;
    if (!shader.preprocess(&resources, DefaultVersion, ENoProfile, false, false, messages, &PreprocessedGLSL, includer)) {
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

bool ShaderCompiler::compile(const cxl::FileSystem* fs, const std::string& file, std::vector<uint32_t>* output) {
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

    return compile(shader_type, text, output);
}

} // gfx

