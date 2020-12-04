#include "command_buffer.hpp"
#include "shader_program.hpp"

#include <core/logging.hpp>
#include <core/hasher.hpp>

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

std::vector<std::shared_ptr<CommandBuffer>> CommandBuffer::create(std::shared_ptr<LogicalDevice>& device,
                                                 Queue::Type queue, vk::CommandBufferLevel level,
                                                 uint32_t num) {
    CXL_CHECK(num > 0);
    std::vector<std::shared_ptr<CommandBuffer>> result;
    try {
        auto pool = device->createCommandPool(queue);
        vk::CommandBufferAllocateInfo alloc_info(pool, level, num);
        std::vector<vk::CommandBuffer> buffers = device->vk().allocateCommandBuffers(alloc_info);
        CXL_CHECK(buffers.size() == num);

        for (uint32_t i = 0; i < buffers.size(); i++) {
            result.push_back(std::shared_ptr<CommandBuffer>(new CommandBuffer()));
            result[i]->command_buffer_ = buffers[i];
            result[i]->device_ = device;
            result[i]->identifier_ = reference_++;
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

CommandBuffer::~CommandBuffer() {
    auto device = device_.lock();
    CXL_DCHECK(device);

    for (auto& [_, pipeline] : state_.pipeline_hash_) {
        device->vk().destroyPipeline(pipeline);
    }
}

void CommandBuffer::bindVertexBuffer(const ComputeBufferPtr& buffer) {
    CXL_CHECK(buffer);
    CXL_CHECK(buffer->size() > 0);
    CXL_CHECK((buffer->usage() & vk::BufferUsageFlagBits::eVertexBuffer) ==
              vk::BufferUsageFlagBits::eVertexBuffer);
    CXL_CHECK(state_.in_render_pass_);
    CXL_CHECK(state_.shader_program_);
    state_.bind_call_.buffers.push_back(buffer->vk());
    state_.bind_call_.offsets.push_back(0);
}

void CommandBuffer::bindIndexBuffer(const ComputeBufferPtr& buffer) {
    CXL_CHECK(buffer);
    CXL_CHECK(buffer->size() > 0);
    CXL_CHECK((buffer->usage() & vk::BufferUsageFlagBits::eIndexBuffer) ==
              vk::BufferUsageFlagBits::eIndexBuffer);
    CXL_CHECK(state_.in_render_pass_);
    CXL_CHECK(state_.shader_program_);
    state_.bind_call_.index_buffer = buffer->vk();
}

void CommandBuffer::bindTexture(uint32_t set, uint32_t binding, const ComputeTexturePtr& texture) {
    CXL_DCHECK(texture);
    state_.pipeline_resources_.descriptors[set].bindings[binding].image_info =
        texture->image_info();
    state_.pipeline_resources_.descriptors[set].bindings[binding].type = texture->type();
    descriptor_flags_ |= 1 << set;
}

void CommandBuffer::bindUniformBuffer(uint32_t set, uint32_t binding,
                                      const ComputeBufferPtr& buffer) {
    CXL_DCHECK(buffer);
    state_.pipeline_resources_.descriptors[set].bindings[binding].buffer_info = buffer->info();
    state_.pipeline_resources_.descriptors[set].bindings[binding].type = buffer->type();
    descriptor_flags_ |= 1 << set;
}

void CommandBuffer::pushConstants(const void* data, vk::DeviceSize offset, vk::DeviceSize size) {
    CXL_DCHECK(offset + size < 256);
    uint8_t* ptr = &state_.pipeline_resources_.push_constants[offset];
    memcpy(ptr, data, size);
    setChanged(kPushConstantBit);
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

void CommandBuffer::beginRenderPass(const RenderPassInfo& render_pass_info) const {
    auto offset = render_pass_info.offset;
    auto extent = render_pass_info.extent;
    vk::RenderPassBeginInfo info(
        /*render_pass*/ render_pass_info.render_pass,
        /*frame_buffer*/ render_pass_info.frame_buffer.get(),
        /*render area*/ vk::Rect2D(offset, extent),
        /*clear_value_count*/ render_pass_info.clear_values.size(),
        /*clear_values*/ render_pass_info.clear_values.data());

    // Update the state.
    state_.in_render_pass_ = true;
    state_.render_pass_ = render_pass_info.render_pass;
    state_.scissor_ = vk::Rect2D(offset, extent);
    state_.current_subpass_ = 0;
    state_.msaa_samples_ = render_pass_info.textures[0]->samples();
    state_.num_subpasses_ = render_pass_info.num_subpasses;

    // Actually begin the render pass.
    setViewPort(vk::Viewport(offset.x, offset.y, extent.width, extent.height, 0.f, 1.f));
    command_buffer_.beginRenderPass(info, vk::SubpassContents::eInline);
}

void CommandBuffer::nextSubPass() const {
    // TODO.
    CXL_DCHECK(state_.current_subpass_ < state_.num_subpasses_ - 1);
    state_.current_subpass_++;
    command_buffer_.nextSubpass(vk::SubpassContents::eInline);
}

void CommandBuffer::endRenderPass() const {
    state_.in_render_pass_ = false;
    state_.current_subpass_ = 0;
    state_.num_subpasses_ = 0;
    command_buffer_.endRenderPass();
}

void CommandBuffer::setViewPort(vk::Viewport viewport) const {
    state_.viewport_ = viewport;
    command_buffer_.setViewport(0, 1, &viewport);
}

void CommandBuffer::setProgram(ShaderProgramPtr program) {
    if (state_.shader_program_ == program) {
        return;
    }

    CXL_CHECK(program);
    CXL_CHECK(state_.in_render_pass_ ==
              (program->bind_point() == vk::PipelineBindPoint::eGraphics));

    if (!state_.shader_program_) {
        descriptor_flags_ = ~0u;
        setChanged(kPushConstantBit);
    }
    // If we get here, then we know the old program and the new program are
    // different, and we have to find the first descriptor set which differs
    // between the two of them and dirty those flags.
    else {
        const ShaderPipeline& new_pipeline = program->pipeline();
        const ShaderPipeline& old_pipeline = state_.shader_program_->pipeline();

        if (new_pipeline.push_constant_layout_hash() != old_pipeline.push_constant_layout_hash()) {
            setChanged(kPushConstantBit);
        }

        for (uint32_t i = 0; i < 32; i++) {
            auto new_descriptor_layout = new_pipeline.descriptor_layout(i);
            auto old_descriptor_layout = old_pipeline.descriptor_layout(i);

            // TODO: This equality check needs to fixed.
            if (new_descriptor_layout != old_descriptor_layout) {
                descriptor_flags_ |= ~((1 << i) - 1);
            }
        }
    }

    state_.shader_program_ = program;
    state_.pipeline_ = vk::Pipeline();
    setChanged(kPipelineBit);
}

void CommandBuffer::setDepth(bool test, bool write) {
    state_.depth_stencil_.depthTestEnable = test;
    state_.depth_stencil_.depthWriteEnable = write;
}

void CommandBuffer::dispatch(uint32_t group_x, uint32_t group_y, uint32_t group_z) {
    prepareComputePipelineData();
    command_buffer_.dispatch(group_x, group_y, group_z);
}

void CommandBuffer::dispatchBase(uint32_t base_x, uint32_t base_y, uint32_t base_z,
                                 uint32_t group_x, uint32_t group_y, uint32_t group_z) {
    CXL_DCHECK(group_x > 0 && group_y > 0 && group_z > 0)
        << group_x << " " << group_y << " " << group_z;
    prepareComputePipelineData();
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

void CommandBuffer::transitionImageLayout(vk::Image& image, vk::Format, vk::ImageAspectFlagBits aspect, vk::ImageLayout old_layout,
                                          vk::ImageLayout new_layout) {
    vk::ImageSubresourceRange resource_range(aspect, 0, 1, 0, 1);

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
            break;
            // throw std::invalid_argument("Unsupported source layout transition!" +
            //                             vk::to_string(old_layout));
    }

    switch (new_layout) {
        // Catch errors here - we cannot go back to undefined or pre-initialized once
        // the image already has a valid layout.
        case vk::ImageLayout::eUndefined:
        case vk::ImageLayout::ePreinitialized: {
            CXL_CHECK(false) << "Cannot transition to undefined or preinitialized";
            break;
        }
        case vk::ImageLayout::eDepthAttachmentOptimal: {
            barrier.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite);
            destination_stage = vk::PipelineStageFlagBits::eAllGraphics;
            break;
        }
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
            break;
            // throw std::invalid_argument("Unsupported destination layout transition!" +
            //                             vk::to_string(new_layout));
    }
    command_buffer_.pipelineBarrier(source_stage, destination_stage, vk::DependencyFlags(), 0,
                                    nullptr, 0, nullptr, 1, &barrier);
}

void CommandBuffer::draw(uint32_t num_vertices, uint32_t instance_count, uint32_t first_vertex,
                         uint32_t first_instance) {
    CXL_DCHECK(state_.shader_program_ &&
               state_.shader_program_->bind_point() == vk::PipelineBindPoint::eGraphics);
    CXL_DCHECK(state_.in_render_pass_) << "Not in render pass!";

    prepareGraphicsPipelineData();
    command_buffer_.draw(num_vertices, instance_count, first_vertex, first_instance);
}

void CommandBuffer::drawIndexed(uint32_t num_indices) {
    CXL_DCHECK(state_.shader_program_ &&
               state_.shader_program_->bind_point() == vk::PipelineBindPoint::eGraphics);
    CXL_DCHECK(state_.in_render_pass_) << "Not in render pass!";

    prepareGraphicsPipelineData();
    command_buffer_.drawIndexed(num_indices, 1, 0, 0, 0);
}

void CommandBuffer::prepareGraphicsPipelineData() {
    if (getAndClear(kPipelineBit)) {
        state_.generateGraphicsPipeline(device_.lock());
        command_buffer_.bindPipeline(vk::PipelineBindPoint::eGraphics, state_.pipeline_);
    }

    if (state_.bind_call_.index_buffer != vk::Buffer()) {
        command_buffer_.bindIndexBuffer(state_.bind_call_.index_buffer, 0, vk::IndexType::eUint32);
        state_.bind_call_.index_buffer = vk::Buffer();
    }

    if (state_.bind_call_.buffers.size() > 0) {
        auto buffers = state_.bind_call_.buffers.data();
        auto num = state_.bind_call_.buffers.size();
        auto offsets = state_.bind_call_.offsets.data();
        command_buffer_.bindVertexBuffers(/*first_binding*/ 0, /*binding_count*/ num, buffers,
                                          offsets);
        state_.bind_call_.buffers.clear();
        state_.bind_call_.offsets.clear();
    }

    if (getAndClear(kPushConstantBit)) {
        const auto& pipeline = state_.shader_program_->pipeline();
        auto vk_layout = pipeline.vk();
        const auto& push_ranges = pipeline.push_ranges();
        for (const auto& range : push_ranges) {
            command_buffer_.pushConstants(vk_layout, range.stageFlags, range.offset, range.size,
                                          state_.pipeline_resources_.push_constants + range.offset);
        }
    }

    prepareDescriptorSets();
}

void CommandBuffer::prepareComputePipelineData() {
    if (getAndClear(kPipelineBit)) {
        state_.generateComputePipeline(device_.lock());
        command_buffer_.bindPipeline(vk::PipelineBindPoint::eCompute, state_.pipeline_);
    }

    prepareDescriptorSets();
}

void CommandBuffer::prepareDescriptorSets() {
    CXL_DCHECK(state_.shader_program_);

    const auto& shader_pipeline = state_.shader_program_->pipeline();
    uint32_t descriptors_to_update = descriptor_flags_ & shader_pipeline.descriptor_set_mask();

    for (uint32_t i = 0; i < 32; i++) {
        if (descriptors_to_update & 1 << i) {
            prepareDescriptorSet(i);
        }
    }
    descriptor_flags_ &= ~descriptors_to_update;
}

void CommandBuffer::prepareDescriptorSet(uint32_t index) {
    auto device = device_.lock();
    CXL_DCHECK(device);
    CXL_DCHECK(state_.shader_program_);

    const auto& shader_pipeline = state_.shader_program_->pipeline();

    auto descriptor_layout = shader_pipeline.descriptor_layout(index);
    auto descriptor_info = descriptor_layout->info();
    auto bind_point = state_.shader_program_->bind_point();

    cxl::Hasher hasher(0);
    vk::DescriptorSet vk_set;
    uint32_t binding_count = descriptor_info.bindingCount;
    for (uint32_t i = 0; i < binding_count; i++) {
        vk::DescriptorSetLayoutBinding binding = descriptor_info.pBindings[i];
        vk::DescriptorType type = binding.descriptorType;

        const auto& curr_data = state_.pipeline_resources_.descriptors[index].bindings[i];
        CXL_DCHECK(curr_data.type == type)
            << vk::to_string(curr_data.type) << " | " << vk::to_string(type) << " Set: " << index << " Binding: " << i;

        hasher.hash(curr_data.identifier);
        hasher.hash(curr_data.type);
        hasher.hash(index);
        hasher.hash(i);
    }

    uint32_t hash = hasher.get_hash();

    if (!state_.descriptor_hash_.count(hash)) {
        vk_set = descriptor_layout->createDescriptorSet()->vk();
        state_.descriptor_hash_[hash] = vk_set;
        writeDescriptorSet(index, vk_set);
    } else {
        vk_set = state_.descriptor_hash_[hash];
    }

    command_buffer_.bindDescriptorSets(bind_point, shader_pipeline.vk(), index, 1, &vk_set, 0,
                                       nullptr);
}

void CommandBuffer::writeDescriptorSet(uint32_t index, vk::DescriptorSet vk_set) {
    auto device = device_.lock();
    CXL_DCHECK(device);

    const auto& shader_pipeline = state_.shader_program_->pipeline();
    auto descriptor_layout = shader_pipeline.descriptor_layout(index);
    auto descriptor_info = descriptor_layout->info();

    std::vector<vk::WriteDescriptorSet> write_infos;

    for (uint32_t i = 0; i < descriptor_info.bindingCount; i++) {
        vk::DescriptorSetLayoutBinding binding = descriptor_info.pBindings[i];
        vk::DescriptorType type = binding.descriptorType;

        const auto* curr_data = &state_.pipeline_resources_.descriptors[index].bindings[i];
        CXL_DCHECK(curr_data->type == type);

        vk::WriteDescriptorSet write_info;
        switch (type) {
            case vk::DescriptorType::eStorageBuffer:
            case vk::DescriptorType::eUniformBuffer: {
                auto& buffer = curr_data->buffer_info.buffer;
                CXL_DCHECK(buffer);
                write_info = vk::WriteDescriptorSet(vk_set, i, 0, 1, curr_data->type, nullptr,
                                                    &curr_data->buffer_info);
                break;
            }
            case vk::DescriptorType::eStorageBufferDynamic:
            case vk::DescriptorType::eUniformBufferDynamic:
                break;
            case vk::DescriptorType::eCombinedImageSampler: {
                write_info = vk::WriteDescriptorSet(vk_set, i, 0, 1, curr_data->type,
                                                    &curr_data->image_info);
                CXL_DCHECK(curr_data->image_info.imageView);
                break;
            }
        }

        write_infos.push_back(write_info);
    }

    device->vk().updateDescriptorSets(write_infos.size(), write_infos.data(), 0, nullptr);
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