// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_DESCRIPTOR_ALLOCATOR_HPP_
#define GRAPHICS_VK_WRAPPERS_DESCRIPTOR_ALLOCATOR_HPP_

#include "vk_wrappers/descriptor_set.hpp"
#include "vk_wrappers/logical_device.hpp"

namespace gfx {

class DescriptorSetLayout {
   public:
    DescriptorSetLayout(std::shared_ptr<LogicalDevice> device,
                        vk::DescriptorSetLayoutCreateInfo info);
    ~DescriptorSetLayout();

    vk::DescriptorSetLayout vk() const { return layout_; }

    std::vector<DescriptorSet> createDescriptorSets(uint32_t num = 1);
    std::shared_ptr<DescriptorSet> createDescriptorSet();

   private:
    class Allocator {
       public:
        Allocator(std::shared_ptr<LogicalDevice> device, vk::DescriptorSetLayoutCreateInfo info,
                  uint32_t num_per_pool)
            : device_(device), create_info_(info), num_per_pool_(num_per_pool) {
            addPool();
        }

        ~Allocator();

        std::vector<vk::DescriptorSet> allocate(vk::DescriptorSetLayout layout, uint32_t num);

       private:
        void addPool();

        std::weak_ptr<LogicalDevice> device_;
        vk::DescriptorSetLayoutCreateInfo create_info_;
        std::vector<vk::DescriptorPool> pools_;

        std::vector<uint32_t> remaining_allocations_;
        uint32_t num_per_pool_;
        uint32_t current_pool_index_;
        std::mutex mutex_;
    };

    std::weak_ptr<LogicalDevice> device_;
    std::unique_ptr<Allocator> allocator_;
    vk::DescriptorSetLayoutCreateInfo create_info_;
    vk::DescriptorSetLayout layout_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_DESCRIPTOR_ALLOCATOR_HPP_
