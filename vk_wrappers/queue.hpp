// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_QUEUE_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_QUEUE_HPP_

#include "vk_wrappers/vulkan.hpp"
#include <mutex>

namespace gfx {
class CommandBuffer;
class Queue {
public:

    enum class Type {
        kCompute,
        kGraphics,
        kPresent,
        kTransfer
    };

    Queue(vk::Queue queue);

    void submit(const vk::SubmitInfo submit_info, const vk::Fence& fence) const;

    void submit(const std::vector<vk::SubmitInfo>& submit_info, const vk::Fence& fence) const;

    void submit(std::shared_ptr<CommandBuffer> command_buffer) const;
    void submit(const gfx::CommandBuffer& command_buffer, const vk::Fence& fence = vk::Fence()) const;

    void present(const vk::PresentInfoKHR& present_info) const;

    void waitIdle() const;

private:
    vk::Queue queue_;
};

} // gfx

#endif // DALI_GRAPHICS_VK_WRAPPERS_QUEUE_HPP_