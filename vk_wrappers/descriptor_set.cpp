#include "descriptor_set.hpp"
namespace gfx {

DescriptorSet::~DescriptorSet() {
    auto device = device_.lock();
    CXL_DCHECK(device) << "Device was deleted before descriptor set!";

    //device->vk().freeDescriptorSets(descriptor_set_);
}

void DescriptorSet::set_buffer(uint32_t index, std::shared_ptr<ComputeBuffer> buffer) const{
    const auto& device = device_.lock();
    CXL_DCHECK(device);
    CXL_DCHECK(index < create_info_.bindingCount);

    vk::DescriptorBufferInfo buffer_info(buffer->vk(), 0, buffer->size());
    vk::WriteDescriptorSet write_info(descriptor_set_,
                                      index, 0, 1, 
                                      buffer->type(),
                                      nullptr, &buffer_info);

    device->vk().updateDescriptorSets(1, &write_info, 0, nullptr);
}

void DescriptorSet::set_texture(uint32_t index, std::shared_ptr<ComputeTexture> texture) const {
    const auto& device = device_.lock();
    CXL_DCHECK(device);
    CXL_DCHECK(index < create_info_.bindingCount);

    auto image_info = texture->image_info();
    vk::WriteDescriptorSet write_info(descriptor_set_,
                                      index, 0, 1, 
                                      texture->type(),
                                      &image_info);
    device->vk().updateDescriptorSets(1, &write_info, 0, nullptr);
}

} // gfx