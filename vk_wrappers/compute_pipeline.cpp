#include "vk_wrappers/compute_pipeline.hpp"

#include <iostream>
#include <string>

#include "logging/logging.hpp"
#include "streaming/file_stream.hpp"

namespace gfx {

namespace {
// This function recursively checks the included headers of each file
// to see if it has been modified. If any file within the recursive stack
// has been modified, then the whole compute program needs to be recompiled.
void recursiveIncludeCheck(const cxl::FileSystem* fs, const std::string& file_name,
                           uint64_t last_run, bool* recompile) {
    if (!fs->fileExists(file_name)) {
        return;
    }

    *recompile |= fs->fileModified(file_name, last_run);
    if (fs->fileModified(file_name, last_run)) {
        CXL_LOG(INFO) << "File modified: " << file_name.c_str();
    }

    std::string line;
    cxl::FileStream stream(fs, file_name);
    while (stream.getLine(line)) {
        uint64_t pos = line.find("#include");
        if (pos != std::string::npos) {
            std::string file = line.substr(pos + 8);
            file.erase(std::remove(file.begin(), file.end(), '\"'), file.end());
            file.erase(std::remove(file.begin(), file.end(), ' '), file.end());
            if (file.find(".hpp") != std::string::npos) {
                file = "../Interface/" + file;
            }
            recursiveIncludeCheck(fs, file, last_run, recompile);
        }
    }
}
}  // namespace

ComputePipeline::ComputePipeline(std::shared_ptr<LogicalDevice> device, const cxl::FileSystem* fs,
                                 const std::string& file)
    : Pipeline(device) {
    CXL_DCHECK(fs);
    try {
        CXL_VLOG(7) << "Creating compute pipeline " << file << std::endl;
        shader_ = std::make_unique<ShaderModule>(device, fs, file);
        __initialize(shader_->spir_v());
        CXL_VLOG(7) << "\n\n" << file << std::endl;
    } catch (...) {
    }
}

ComputePipeline::ComputePipeline(std::shared_ptr<LogicalDevice> device,
                                 const std::vector<uint32_t>& shader)
    : Pipeline(device) {
    shader_ = std::make_unique<ShaderModule>(device, vk::ShaderStageFlagBits::eCompute, shader);
    __initialize(shader);
}

void ComputePipeline::__initialize(const std::vector<uint32_t>& spir_v) {
    auto device = device_.lock();
    CXL_DCHECK(device);

    // Create reflection module.
    std::vector<std::vector<uint32_t>> spirv_code = {spir_v};
    reflection_ = std::make_unique<Reflection>(device, spirv_code);
    CXL_DCHECK(reflection_) << "Could not generate reflection module.";

    // Generate descriptor set layouts.
    layouts_ = reflection_->createLayouts();
    CXL_DCHECK(layouts_.size() > 0);

    push_constants_ = reflection_->createPushConstants();

    // Get raw vulkan layouts
    std::vector<vk::DescriptorSetLayout> vk_layouts;
    for (auto& layout : layouts_) {
        vk_layouts.push_back(layout->vk());
    }

    // The pipeline layout allows the pipeline to access descriptor sets.
    vk::PipelineLayoutCreateInfo layout_info({}, vk_layouts.size(), vk_layouts.data(),
                                             push_constants_.size(), push_constants_.data());
    pipeline_layout_ = device->vk().createPipelineLayout(layout_info);

    // Now create the actual pipeline.
    // TODO: Actually make use of pipeline cache.
    vk::ComputePipelineCreateInfo pipeline_info({}, shader_->create_info(), pipeline_layout_);
    pipeline_ = device->vk().createComputePipelines(vk::PipelineCache(), {pipeline_info})[0];
}

ComputePipeline::~ComputePipeline() {
    shader_.reset();
    if (auto device = device_.lock()) {
        for (auto& layout : layouts_) {
            layout.reset();
        }
        device->vk().destroy(pipeline_layout_);
        device->vk().destroy(pipeline_);
    }
}
}  // namespace gfx