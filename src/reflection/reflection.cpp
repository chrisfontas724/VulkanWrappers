#include "reflection.hpp"
#include "spirv_reflect.h"
#include <core/logging.hpp>
#include <map>

namespace gfx {

namespace {
static const std::map<SpvReflectDescriptorType, vk::DescriptorType> type_map {
  { SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER,                vk::DescriptorType::eSampler},  
  { SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, vk::DescriptorType::eCombinedImageSampler},
  { SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          vk::DescriptorType::eSampledImage},
  { SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE,          vk::DescriptorType::eStorageImage},
  { SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   vk::DescriptorType::eUniformTexelBuffer},
  { SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   vk::DescriptorType::eStorageTexelBuffer},
  { SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         vk::DescriptorType::eUniformBuffer},
  { SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER,         vk::DescriptorType::eStorageBuffer},
  { SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, vk::DescriptorType::eUniformBufferDynamic},
  { SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, vk::DescriptorType::eStorageBufferDynamic},
  { SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       vk::DescriptorType::eInputAttachment},
};

static const std::map<SpvReflectShaderStageFlagBits, vk::ShaderStageFlagBits> stage_map {
  { SPV_REFLECT_SHADER_STAGE_VERTEX_BIT,                  vk::ShaderStageFlagBits::eVertex} ,
  { SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT,    vk::ShaderStageFlagBits::eTessellationControl},
  { SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, vk::ShaderStageFlagBits::eTessellationEvaluation},
  { SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT,                vk::ShaderStageFlagBits::eGeometry},
  { SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT,                vk::ShaderStageFlagBits::eFragment},
  { SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT,                 vk::ShaderStageFlagBits::eCompute},
};

vk::DescriptorSetLayoutBinding convertBinding(const SpvReflectDescriptorBinding* binding, vk::ShaderStageFlagBits stage) {
    return vk::DescriptorSetLayoutBinding(binding->binding,                      // Binding index
                                          type_map.at(binding->descriptor_type), // Binding type
                                          1, stage);
}

vk::PushConstantRange convertPushConstant(const SpvReflectBlockVariable& push_constant, vk::ShaderStageFlagBits stages) {
    return vk::PushConstantRange(stages, push_constant.offset, push_constant.size);
}
} // namespace

struct Reflection::Impl {
    std::vector<spv_reflect::ShaderModule*> modules;
    ~Impl() {
        for (auto& module : modules) {
            delete module;
        }
    }
};

Reflection::~Reflection() {
    impl_.reset();
}

Reflection::Reflection(std::shared_ptr<LogicalDevice> device, const std::vector<std::vector<uint32_t>>& spirv)
: device_(device) {
    impl_ = std::make_unique<Impl>();
    CXL_DCHECK(impl_);

    for (auto& code : spirv) {
        impl_->modules.push_back(new spv_reflect::ShaderModule(code));
    }
}

std::vector<vk::PushConstantRange> Reflection::createPushConstants() {
    std::map<std::string, vk::PushConstantRange> constants_map;

    // Convert to Vulkan representation.
    std::vector<vk::PushConstantRange> result;
    CXL_VLOG(7) << "There are " << impl_->modules.size() << " modules to check.";
    for (auto& module : impl_->modules) {
        // Get module and shader stage.
        auto shader_stage = stage_map.at(module->GetShaderStage());
        CXL_VLOG(7) << "Shader stage is: " << vk::to_string(shader_stage);

        // Get push constants
        uint32_t num_push_constants;
        module->EnumeratePushConstantBlocks(&num_push_constants, nullptr);

        SpvReflectBlockVariable* push_constants = new SpvReflectBlockVariable[num_push_constants];
        module->EnumeratePushConstantBlocks(&num_push_constants, &push_constants);

        CXL_VLOG(7) << "Stage has " << num_push_constants << " constants.";
        for (uint32_t i = 0; i < num_push_constants; i++) {

            auto& push_constant = push_constants[i];
            SpvReflectTypeDescription* description = push_constant.type_description;
            if (constants_map.count(push_constant.name)) {
                CXL_VLOG(7) << "Push constant already mapped!";
                constants_map[push_constant.name].stageFlags |= shader_stage;
                continue;
            }

            CXL_VLOG(7) << "Push Constant Name: " << push_constant.name;
            CXL_VLOG(7) << "Push Constant Offset: " << push_constant.offset;
            CXL_VLOG(7) << "Push Constant Absolute Offset: " << push_constant.absolute_offset;
            CXL_VLOG(7) << "Push Constant Size: " << push_constant.size;
            CXL_VLOG(7) << "Push Constant Padded Size: " << push_constant.padded_size;
            CXL_VLOG(7) << "Push Constant Description: " << description->type_name;
            constants_map[push_constant.name] = convertPushConstant(push_constant, shader_stage);
        }
    }

    for (auto& iter : constants_map) {
        auto& constant = iter.second;
        result.push_back(iter.second);
    }

    CXL_VLOG(7) << "Returning " << result.size() << " push constant(s).";
    return result;
}

std::vector<std::shared_ptr<DescriptorSetLayout>> Reflection::createLayouts() {
    auto device = device_.lock();
    CXL_DCHECK(device);
    CXL_DCHECK(impl_);
    CXL_DCHECK(impl_->modules.size() > 0);

    // A mapping of set ids to bindings.
    std::map<uint32_t, std::vector<vk::DescriptorSetLayoutBinding>> binding_map;

    for (auto& module : impl_->modules) {
        auto shader_stage = stage_map.at(module->GetShaderStage());
        CXL_VLOG(7) << "Shader stage is: " << vk::to_string(shader_stage);

        // Get descriptor sets.
        uint32_t num_descriptor_sets;
        module->EnumerateDescriptorSets(&num_descriptor_sets, nullptr);
        CXL_VLOG(7) << "Num Descriptor Sets: " << num_descriptor_sets;

        SpvReflectDescriptorSet* descriptor_sets = new SpvReflectDescriptorSet[num_descriptor_sets];
        module->EnumerateDescriptorSets(&num_descriptor_sets, &descriptor_sets);

        for (uint32_t i = 0; i < num_descriptor_sets; i++) {
            auto& descriptor_set = descriptor_sets[i];
            CXL_VLOG(7) << "  Set binding count: " << descriptor_set.binding_count;
            CXL_VLOG(7) << "  Set index: " << descriptor_set.set;

            for (uint32_t j = 0; j < descriptor_set.binding_count; j++) {
                auto& binding =  descriptor_set.bindings[j];
                CXL_DCHECK(binding) << "Binding was null";
                CXL_VLOG(7) << "BINDING NAME: " << binding->name;
                CXL_VLOG(7) << "BINDING INDEX: " << binding->binding;
                CXL_VLOG(7) << "BINDING TYPE: " << binding->descriptor_type;

                binding_map[descriptor_set.set].push_back(convertBinding(binding, shader_stage));
            }
        }
    }

    std::vector<std::shared_ptr<DescriptorSetLayout>> layouts;
    layouts.resize(binding_map.size());

    uint32_t i = 0;
    for (auto& iter : binding_map) {
        uint32_t binding_count = iter.second.size();
        auto bindings = new vk::DescriptorSetLayoutBinding[iter.second.size()];
        memcpy(bindings, iter.second.data(), sizeof(vk::DescriptorSetLayoutBinding) * binding_count);
        vk::DescriptorSetLayoutCreateInfo layout_create_info({}, binding_count, bindings);
        layouts[i] = std::make_shared<DescriptorSetLayout>(device, layout_create_info);
        i++;
    }

    return layouts;
}

} //