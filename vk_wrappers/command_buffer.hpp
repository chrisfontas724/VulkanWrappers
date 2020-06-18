// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_HPP_
#define GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_HPP_

#include <glm/glm.hpp>

#include "vk_wrappers/compute_pipeline.hpp"
#include "vk_wrappers/graphics_pipeline.hpp"
#include "vk_wrappers/logical_device.hpp"
#include "vk_wrappers/render_pass.hpp"

namespace gfx {
class CommandBuffer {
   public:
    static std::shared_ptr<CommandBuffer> create(LogicalDevicePtr device, Queue::Type queue,
                                                 vk::CommandBufferLevel level);

    static std::vector<CommandBuffer> create(LogicalDevicePtr& device, Queue::Type queue,
                                             vk::CommandBufferLevel level, uint32_t num);

    const vk::CommandBuffer& vk() const { return command_buffer_; }

    void reset() const;
    void beginRecording() const;

    void beginRenderPass(const RenderPassInfo& render_pass_info,
                         const glm::vec4& clear_color) const;

    void nextSubPass() const;

    void endRenderPass() const;

    void setViewPort(vk::Viewport viewport);

    void bindPipeline(PipelinePtr pipeline);

    void bindDescriptorSet(DescriptorSetPtr set, uint32_t index);

    void pushConstants(const void* data, uint32_t index);

    template <typename T>
    void pushConstants(T& value, uint32_t index) {
        pushConstants(&value, index);
    }

    void setDepth(bool test, bool write);

    void dispatch(uint32_t group_x, uint32_t group_y, uint32_t group_z);

    void dispatchBase(uint32_t base_x, uint32_t base_y, uint32_t base_z, uint32_t group_x,
                      uint32_t group_y, uint32_t group_z);

    void draw(uint32_t num_vertices, uint32_t instance_count = 1, uint32_t first_vertex = 0,
              uint32_t first_instance = 0);

    void drawIndexed(uint32_t num_indices);

    template <typename T>
    void updateBuffer(ComputeBufferPtr buffer, const std::vector<T>& vec, vk::DeviceSize offset) {
        uint64_t size = sizeof(T) * vec.size();
        CXL_DCHECK(size % 4 == 0) << "Total memory for |updateBuffer| must be a multple of 4";
        command_buffer_.updateBuffer(buffer->vk(), offset, sizeof(T) * vec.size(), vec.data());
    }

    void fillBuffer(std::shared_ptr<gfx::ComputeBuffer> buffer, vk::DeviceSize offset,
                    vk::DeviceSize num_bytes, uint32_t data) {
        CXL_DCHECK(num_bytes % 4 == 0);
        command_buffer_.fillBuffer(buffer->vk(), offset, num_bytes, data);
    }

    void copyBufferToImage(ComputeBufferPtr buffer, vk::Image image, uint32_t width,
                           uint32_t height);
    void copyImageToBuffer(ComputeTexturePtr texture, ComputeBufferPtr buffer);
    void copyImageToImage(ComputeTexturePtr src, ComputeTexturePtr dst);
    void clearColorImage(ComputeTexturePtr image, std::array<float, 4> color);

    void transitionImageLayout(vk::Image& image, vk::Format, vk::ImageLayout old_layout,
                               vk::ImageLayout new_layout);

    void blit(ComputeTexturePtr src, ComputeTexturePtr dst, vk::Filter filter);

    void endRecording() const { command_buffer_.end(); }

    bool has_recording() const;

   private:
    CommandBuffer() {}
    std::weak_ptr<LogicalDevice> device_;
    vk::CommandBuffer command_buffer_;
    uint32_t identifier_;

    static uint32_t reference_;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_HPP_