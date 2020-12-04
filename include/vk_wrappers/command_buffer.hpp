// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_HPP_
#define GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_HPP_

#include <glm/glm.hpp>

#include "command_buffer_state.hpp"
#include "logical_device.hpp"
#include "render_pass.hpp"
#include "compute_buffer.hpp"

namespace gfx {
class CommandBuffer {
   public:
    static std::shared_ptr<CommandBuffer> create(LogicalDevicePtr device, Queue::Type queue,
                                                 vk::CommandBufferLevel level);

    static std::vector<std::shared_ptr<CommandBuffer>> create(LogicalDevicePtr& device, Queue::Type queue,
                                             vk::CommandBufferLevel level, uint32_t num);

    ~CommandBuffer();

    const vk::CommandBuffer& vk() const { return command_buffer_; }

    void reset() const;
    void beginRecording() const;

    void beginRenderPass(const RenderPassInfo& render_pass_info) const;

    void nextSubPass() const;

    void endRenderPass() const;

    void setDefaultState(CommandBufferState::DefaultState state) {
        state_.set_default_state(state);
        changed_flags_ |= kPipelineBit;
    }

    void setVertexAttribute(uint32_t binding, uint32_t location, vk::Format format) {
        state_.vertex_description_.bindings[binding].formats[location] = format;
    }

    void setProgram(ShaderProgramPtr program);

    void bindVertexBuffer(const ComputeBufferPtr& buffer);

    void bindIndexBuffer(const ComputeBufferPtr& buffer);

    void bindTexture(uint32_t set, uint32_t binding, const ComputeTexturePtr& texture);
    void bindUniformBuffer(uint32_t set, uint32_t binding, const ComputeBufferPtr& buffer);

    template <typename T>
    void pushConstants(const T& data, vk::DeviceSize offset = vk::DeviceSize(0)) {
        pushConstants(&data, offset, static_cast<vk::DeviceSize>(sizeof(T)));
    }

    void pushConstants(const void* data, vk::DeviceSize offset, vk::DeviceSize size);

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

    void transitionImageLayout(vk::Image& image, vk::Format, vk::ImageAspectFlagBits aspect, vk::ImageLayout old_layout,
                               vk::ImageLayout new_layout);

    void blit(ComputeTexturePtr src, ComputeTexturePtr dst, vk::Filter filter);

    void endRecording() const { command_buffer_.end(); }

    bool has_recording() const;

   private:
    CommandBuffer() {}
    void setViewPort(vk::Viewport viewport) const;
    void prepareGraphicsPipelineData();
    void prepareComputePipelineData();
    void prepareDescriptorSets();
    void prepareDescriptorSet(uint32_t index);
    void writeDescriptorSet(uint32_t index, vk::DescriptorSet vk_set);

    using Flags = uint32_t;
    enum FlagBits {
        kPipelineBit = 1 << 0,
        kPushConstantBit = 1 << 1,
    };

    void setChanged(uint32_t mask) { changed_flags_ |= mask; }

    uint32_t getAndClear(uint32_t mask) {
        auto result = changed_flags_ & mask;
        changed_flags_ &= ~mask;
        return result;
    }

    Flags changed_flags_ = ~0u;
    Flags descriptor_flags_ = ~0u;

    mutable CommandBufferState state_;
    std::weak_ptr<LogicalDevice> device_;
    vk::CommandBuffer command_buffer_;
    uint32_t identifier_;

    static uint32_t reference_;
    bool boop = false;
};
}  // namespace gfx

#endif  // GRAPHICS_VK_WRAPPERS_COMMAND_BUFFER_HPP_