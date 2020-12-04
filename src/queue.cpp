#include "vk_wrappers/queue.hpp"

#include "logging/logging.hpp"
#include "vk_wrappers/command_buffer.hpp"

namespace gfx {

// TODO: HACK. All of my queues are technically the same, and so they all
// need to share the same mutex. This is bad. Find a way to figure out which
// queues are the same and give them the same mutex or else make all the queues
// unique so they don't run into this problem.
static std::mutex lock_;

Queue::Queue(vk::Queue queue) : queue_(queue) {}

void Queue::submit(const vk::SubmitInfo submit_info, const vk::Fence& fence) const {
    std::lock_guard<std::mutex> lock(lock_);
    vk::Result result = queue_.submit(1, &submit_info, fence);
    if (result != vk::Result::eSuccess) {
        CXL_LOG(ERROR) << "Could not submit info to queue: " << vk::to_string(result);
    }
}

void Queue::submit(const std::vector<vk::SubmitInfo>& submit_info, const vk::Fence& fence) const {
    std::lock_guard<std::mutex> lock(lock_);
    vk::Result result = queue_.submit(submit_info.size(), submit_info.data(), fence);
    if (result != vk::Result::eSuccess) {
        CXL_LOG(ERROR) << "Could not submit info to queue: " << vk::to_string(result);
    }
}

void Queue::submit(std::shared_ptr<CommandBuffer> command_buffer) const {
    vk::Fence fence;
    vk::SubmitInfo submit_info(0, nullptr, {}, 1U, &command_buffer->vk(), 0, nullptr);

    // This is a locking function.
    submit(submit_info, fence);
}

void Queue::submit(const CommandBuffer& command_buffer, const vk::Fence& fence) const {
    vk::SubmitInfo submit_info(0, nullptr, {}, 1U, &command_buffer.vk(), 0, nullptr);

    // This is a locking function.
    submit(submit_info, fence);
}

void Queue::present(const vk::PresentInfoKHR& present_info) const {
    std::lock_guard<std::mutex> lock(lock_);
    queue_.presentKHR(&present_info);
}

void Queue::waitIdle() const {
    std::lock_guard<std::mutex> lock(lock_);
    queue_.waitIdle();
}

}  // namespace gfx