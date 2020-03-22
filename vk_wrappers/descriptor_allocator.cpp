#include "descriptor_allocator.hpp"

namespace gfx {

DescriptorSetLayout::Allocator::~Allocator() {
    auto device = device_.lock();
    CXL_DCHECK(device);
    for (auto& pool : pools_) {
        device->vk().destroyDescriptorPool(pool);
    }
}

void DescriptorSetLayout::Allocator::addPool() {
    auto device = device_.lock();
    CXL_DCHECK(device) << "Device not found";

    std::vector<vk::DescriptorPoolSize> pool_sizes;
    pool_sizes.resize(create_info_.bindingCount);
    for (uint32_t i = 0; i < create_info_.bindingCount; i++) {
        pool_sizes[i].setType(create_info_.pBindings[i].descriptorType);
        pool_sizes[i].setDescriptorCount(num_per_pool_);
    }

    vk::DescriptorPoolCreateInfo pool_info({}, num_per_pool_, pool_sizes.size(), pool_sizes.data());

    pools_.push_back(device->vk().createDescriptorPool(pool_info));
    remaining_allocations_.push_back(num_per_pool_);
    current_pool_index_ = pools_.size() - 1;
}

std::vector<vk::DescriptorSet> DescriptorSetLayout::Allocator::allocate(vk::DescriptorSetLayout layout, uint32_t num) {
    auto device = device_.lock();
    CXL_DCHECK(device) << "Device not found";

    uint32_t num_remaining = remaining_allocations_[current_pool_index_];
    if (num_remaining <= 0) {
        addPool();
    }
    auto& pool = pools_[current_pool_index_];


    std::unique_lock<std::mutex> lock(mutex_);
    vk::DescriptorSetAllocateInfo alloc_info(pool, 1, &layout);
    std::vector<vk::DescriptorSet> sets = device->vk().allocateDescriptorSets(alloc_info);
    lock.unlock();

    remaining_allocations_[current_pool_index_]--;

    return sets;
}

DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<LogicalDevice> device, vk::DescriptorSetLayoutCreateInfo info)
: device_(device)
, create_info_(info) {
    layout_ = device->vk().createDescriptorSetLayout(info);

    // TODO: Add device and variable number.
    allocator_ = std::unique_ptr<Allocator>(new Allocator(device, create_info_, 15));
}

DescriptorSetLayout::~DescriptorSetLayout() {
    auto device = device_.lock();
    device->vk().destroy(layout_);
    allocator_.reset();
}

std::shared_ptr<DescriptorSet> DescriptorSetLayout::createDescriptorSet() {
    std::vector<vk::DescriptorSet> sets = allocator_->allocate(layout_, 1);
    return std::shared_ptr<DescriptorSet>(new DescriptorSet(device_.lock(), sets[0], create_info_));
}

std::vector<DescriptorSet> DescriptorSetLayout::createDescriptorSets(uint32_t num) {
    std::vector<vk::DescriptorSet> sets = allocator_->allocate(layout_, num);
    std::vector<DescriptorSet> result;
    for (auto& set : sets) {
        result.push_back(DescriptorSet(device_.lock(), set, create_info_));
    }
    return result;
}
}