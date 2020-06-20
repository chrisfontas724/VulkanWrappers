#include "vk_wrappers/command_buffer.hpp"

#include "logging/logging.hpp"
#include "vk_wrappers/shader_program.hpp"

namespace gfx {
uint32_t CommandBuffer::reference_ = 0;

std::shared_ptr<CommandBuffer> CommandBuffer::create(std::shared_ptr<LogicalDevice> device,
                                                     Queue::Type queue,
                                                     vk::CommandBufferLevel level) {
    CXL_DCHECK(device);
    std::shared_ptr<CommandBuffer> result = std::shared_ptr<CommandBuffer>(new CommandBuffer);
    try {
        auto pool = device->createCommandPool(queue);
        vk::CommandBufferAllocateInfo alloc_info(pool, level, 1);
        std::vector<vk::CommandBuffer> buffers = device->vk().allocateCommandBuffers(alloc_info);
        CXL_CHECK(buffers.size() == 1);
        auto& buffer = buffers[0];
        result->command_buffer_ = buffers[0];
        result->device_ = device;
        result->identifier_ = reference_++;
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        return result;
    } catch (...) {
        std::cout << "unknown error\n";
        return result;
    }
    return result;
}

std::vector<CommandBuffer> CommandBuffer::create(std::shared_ptr<LogicalDevice>& device,
                                                 Queue::Type queue, vk::CommandBufferLevel level,
                                                 uint32_t num) {
    CXL_CHECK(num > 0);
    std::vector<CommandBuffer> result;
    try {
        auto pool = device->createCommandPool(queue);
        vk::CommandBufferAllocateInfo alloc_info(pool, level, num);
        std::vector<vk::CommandBuffer> buffers = device->vk().allocateCommandBuffers(alloc_info);
        CXL_CHECK(buffers.size() == num);

        for (uint32_t i = 0; i < buffers.size(); i++) {
            result.push_back(CommandBuffer());
            result[i].command_buffer_ = buffers[i];
            result[i].device_ = device;
            result[i].identifier_ = reference_++;
        }
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        return result;
    } catch (...) {
        std::cout << "unknown error\n";
        return result;
    }

    return result;
}

void CommandBuffer::reset() const {
    command_buffer_.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
    state_.has_recording_ = false;
}

void CommandBuffer::beginRecording() const {
    try {
        vk::CommandBufferBeginInfo info(vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr);
        command_buffer_.begin(info);
        state_.has_recording_ = true;
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    } catch (...) {
        std::cout << "unknown error\n";
        exit(-1);
    }
}

void CommandBuffer::beginRenderPass(const RenderPassInfo& render_pass_info,
                                    const glm::vec4& clear_color) const {
    std::array<float, 4> array = {clear_color.x, clear_color.y, clear_color.z, clear_color.w};
    vk::ClearColorValue values(array);
    vk::ClearValue clear_value(values);
    auto offset = render_pass_info.offset;
    auto extent = render_pass_info.extent;

    vk::RenderPassBeginInfo info(
        /*render_pass*/ render_pass_info.render_pass,
        /*frame_buffer*/ render_pass_info.frame_buffer.get(),
        /*render area*/ vk::Rect2D(offset, extent),
        /*clear_value_count*/ 1,
        /*clear_values*/ &clear_value);

    // Update the state.
    state_.in_render_pass_ = true;
    state_.render_pass_ = render_pass_info.render_pass;
    state_.scissor_ = vk::Rect2D(offset, extent);

    // Actually begin the render pass.
    setViewPort(vk::Viewport(offset.x, offset.y, extent.width, extent.height, 0.f, 1.f));
    command_buffer_.beginRenderPass(info, vk::SubpassContents::eInline);
}

void CommandBuffer::nextSubPass() const {
    // TODO.
    command_buffer_.nextSubpass(vk::SubpassContents::eInline);
}

void CommandBuffer::endRenderPass() const {
    state_.in_render_pass_ = false;
    command_buffer_.endRenderPass();
}

void CommandBuffer::setViewPort(vk::Viewport viewport) const {
    state_.viewport_ = viewport;
    command_buffer_.setViewport(0, 1, &viewport);
}

void CommandBuffer::setProgram(ShaderProgramPtr program) {
    CXL_CHECK(program);
    CXL_CHECK(state_.in_render_pass_ ==
              (program->bind_point() == vk::PipelineBindPoint::eGraphics));
    state_.shader_program_ = program;
}

void CommandBuffer::bindDescriptorSet(DescriptorSetPtr set, uint32_t first) {
    // auto pipeline = state_.pipeline_.lock();
    // CXL_DCHECK(pipeline) << "There is no pipeline bound...";
    // auto point = pipeline->bind_point();
    // auto layout = pipeline->pipeline_layout();
    // auto vk_set = set->vk();
    // command_buffer_.bindDescriptorSets(point, layout, first, 1, &vk_set, 0, nullptr);
}

void CommandBuffer::pushConstants(const void* data, uint32_t index) {
    // auto pipeline = state_.pipeline_.lock();
    // CXL_DCHECK(pipeline) << "There is no pipeline bound...";
    // auto layout = pipeline->pipeline_layout();
    // auto range = pipeline->push_constant(index);
    // command_buffer_.pushConstants(layout, range.stageFlags, range.offset, range.size, data);
}

void CommandBuffer::setDepth(bool test, bool write) {
    state_.depth_stencil_.depthTestEnable = test;
    state_.depth_stencil_.depthWriteEnable = write;
}

void CommandBuffer::dispatch(uint32_t group_x, uint32_t group_y, uint32_t group_z) {
    command_buffer_.dispatch(group_x, group_y, group_z);
}

void CommandBuffer::dispatchBase(uint32_t base_x, uint32_t base_y, uint32_t base_z,
                                 uint32_t group_x, uint32_t group_y, uint32_t group_z) {
    CXL_DCHECK(group_x > 0 && group_y > 0 && group_z > 0)
        << group_x << " " << group_y << " " << group_z;
    command_buffer_.dispatchBase(base_x, base_y, base_z, group_x, group_y, group_z);
}

void CommandBuffer::copyBufferToImage(std::shared_ptr<gfx::ComputeBuffer> buffer, vk::Image image,
                                      uint32_t width, uint32_t height) {
    vk::ImageSubresourceLayers layers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
    vk::Offset3D offset(0, 0, 0);
    vk::Extent3D extent(width, height, 1);
    vk::BufferImageCopy region_info(0, 0, 0, layers, offset, extent);
    command_buffer_.copyBufferToImage(buffer->vk(), image, vk::ImageLayout::eTransferDstOptimal, 1,
                                      &region_info);
}

void CommandBuffer::copyImageToBuffer(ComputeTexturePtr texture, ComputeBufferPtr buffer) {
    auto layout = texture->layout();
    if (layout != vk::ImageLayout::eTransferSrcOptimal) {
        texture->transitionImageLayout(*this, vk::ImageLayout::eTransferSrcOptimal);
    }

    // Perform the copy.
    vk::ImageSubresourceLayers layers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
    vk::Offset3D offset(0, 0, 0);
    vk::Extent3D extent(texture->width(), texture->height(), 1);
    vk::BufferImageCopy region_info(0, 0, 0, layers, offset, extent);
    command_buffer_.copyImageToBuffer(texture->image(), texture->layout(), buffer->vk(), 1,
                                      &region_info);

    // transfer back.
    if (layout != texture->layout()) {
        texture->transitionImageLayout(*this, layout);
    }
}

void CommandBuffer::copyImageToImage(ComputeTexturePtr src, ComputeTexturePtr dst) {
    vk::ImageSubresourceLayers layers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);

    auto src_layout = src->layout();
    auto dst_layout = dst->layout();
    src->transitionImageLayout(*this, vk::ImageLayout::eTransferSrcOptimal);
    dst->transitionImageLayout(*this, vk::ImageLayout::eTransferDstOptimal);

    std::array<vk::Offset3D, 2> src_offsets = {vk::Offset3D(0, 0, 0),
                                               vk::Offset3D(src->width(), src->height(), 1)};
    std::array<vk::Offset3D, 2> dst_offsets = {vk::Offset3D(0, 0, 0),
                                               vk::Offset3D(dst->width(), dst->height(), 1)};

    vk::ImageCopy copy_info = {layers, vk::Offset3D(0, 0, 0), layers, vk::Offset3D(0, 0, 0),
                               vk::Extent3D(src->width(), dst->height(), 1)};

    command_buffer_.copyImage(src->image(), src->layout(), dst->image(), dst->layout(), 1,
                              &copy_info);

    src->transitionImageLayout(*this, src_layout);
    dst->transitionImageLayout(*this, dst_layout);
}

void CommandBuffer::clearColorImage(ComputeTexturePtr image, std::array<float, 4> color) {
    vk::ClearColorValue clear_color(color);
    vk::ImageSubresourceRange resource_range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

    auto layout = image->layout();
    image->transitionImageLayout(*this, vk::ImageLayout::eTransferDstOptimal);
    command_buffer_.clearColorImage(image->image(), image->layout(), &clear_color, 1,
                                    &resource_range);
    image->transitionImageLayout(*this, layout);
}

void CommandBuffer::transitionImageLayout(vk::Image& image, vk::Format, vk::ImageLayout old_layout,
                                          vk::ImageLayout new_layout) {
    vk::ImageSubresourceRange resource_range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

    vk::ImageMemoryBarrier barrier(vk::AccessFlags(), vk::AccessFlags(), old_layout, new_layout,
                                   VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, image,
                                   resource_range);
    vk::PipelineStageFlags source_stage;
    vk::PipelineStageFlags destination_stage;

    switch (old_layout) {
        case vk::ImageLayout::eUndefined: {
            barrier.setSrcAccessMask(vk::AccessFlags());
            source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
            break;
        }
        case vk::ImageLayout::eTransferSrcOptimal: {
            barrier.setSrcAccessMask(vk::AccessFlags());
            source_stage = vk::PipelineStageFlagBits::eTransfer;
            break;
        }
        case vk::ImageLayout::eTransferDstOptimal: {
            barrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite);
            source_stage = vk::PipelineStageFlagBits::eTransfer;
            break;
        }
        case vk::ImageLayout::eColorAttachmentOptimal: {
            barrier.setSrcAccessMask(vk::AccessFlags());
            source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
            // TODO: Figure out a more optimal stage.
            break;
        }
        case vk::ImageLayout::eShaderReadOnlyOptimal: {
            barrier.setSrcAccessMask(vk::AccessFlagBits::eShaderRead);
            source_stage = vk::PipelineStageFlagBits::eComputeShader;
            break;
        }
        case vk::ImageLayout::ePresentSrcKHR: {
            barrier.setSrcAccessMask(vk::AccessFlags());
            source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
            // TODO: Figure out a more optimal stage.
            break;
        }
        default:
            throw std::invalid_argument("Unsupported source layout transition!" +
                                        vk::to_string(old_layout));
    }

    switch (new_layout) {
        case vk::ImageLayout::eTransferSrcOptimal: {
            barrier.setDstAccessMask(vk::AccessFlags());
            destination_stage = vk::PipelineStageFlagBits::eTransfer;
            break;
        }
        case vk::ImageLayout::eTransferDstOptimal: {
            barrier.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
            destination_stage = vk::PipelineStageFlagBits::eTransfer;
            break;
        }
        case vk::ImageLayout::eShaderReadOnlyOptimal: {
            barrier.setDstAccessMask(vk::AccessFlagBits::eShaderRead);
            destination_stage = vk::PipelineStageFlagBits::eComputeShader;
            break;
        }
        case vk::ImageLayout::eColorAttachmentOptimal: {
            barrier.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead |
                                     vk::AccessFlagBits::eColorAttachmentWrite |
                                     vk::AccessFlagBits::eShaderRead);
            destination_stage = vk::PipelineStageFlagBits::eAllGraphics;
            break;
        }
        case vk::ImageLayout::ePresentSrcKHR: {
            barrier.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead |
                                     vk::AccessFlagBits::eColorAttachmentWrite |
                                     vk::AccessFlagBits::eShaderRead);
            destination_stage = vk::PipelineStageFlagBits::eAllGraphics;
            break;
        }
        default:
            throw std::invalid_argument("Unsupported destination layout transition!" +
                                        vk::to_string(new_layout));
    }
    command_buffer_.pipelineBarrier(source_stage, destination_stage, vk::DependencyFlags(), 0,
                                    nullptr, 0, nullptr, 1, &barrier);
}

void CommandBuffer::draw(uint32_t num_vertices, uint32_t instance_count, uint32_t first_vertex,
                         uint32_t first_instance) {
    CXL_DCHECK(state_.shader_program_ &&
               state_.shader_program_->bind_point() == vk::PipelineBindPoint::eGraphics);
    CXL_DCHECK(state_.in_render_pass_) << "Not in render pass!";

    if (!boop) {
        state_.generateGraphicsPipeline(device_.lock());
        boop = true;
    }

    command_buffer_.bindPipeline(vk::PipelineBindPoint::eGraphics, state_.pipeline_);
    command_buffer_.draw(num_vertices, instance_count, first_vertex, first_instance);
}

void CommandBuffer::drawIndexed(uint32_t num_indices) {
    CXL_DCHECK(state_.shader_program_ &&
               state_.shader_program_->bind_point() == vk::PipelineBindPoint::eGraphics);
    CXL_DCHECK(state_.in_render_pass_) << "Not in render pass!";
    state_.generateGraphicsPipeline(device_.lock());
    command_buffer_.bindPipeline(vk::PipelineBindPoint::eGraphics, state_.pipeline_);
    command_buffer_.drawIndexed(num_indices, 1, 0, 0, 0);
}

void CommandBuffer::blit(ComputeTexturePtr src, ComputeTexturePtr dst, vk::Filter filter) {
    vk::ImageSubresourceLayers layers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);

    std::array<vk::Offset3D, 2> src_offsets = {vk::Offset3D(0, 0, 0),
                                               vk::Offset3D(src->width(), src->height(), 1)};
    std::array<vk::Offset3D, 2> dst_offsets = {vk::Offset3D(0, 0, 0),
                                               vk::Offset3D(dst->width(), dst->height(), 1)};

    vk::ImageBlit blit_info = {layers, src_offsets, layers, dst_offsets};

    command_buffer_.blitImage(src->image(), src->layout(), dst->image(), dst->layout(), 1,
                              &blit_info, filter);
}

bool CommandBuffer::has_recording() const { return state_.has_recording_; }

}  // namespace gfx