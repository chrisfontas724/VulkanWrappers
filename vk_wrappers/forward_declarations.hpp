// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef DALI_GRAPHICS_VK_WRAPPERS_FORWARD_DECLARATIONS_HPP_
#define DALI_GRAPHICS_VK_WRAPPERS_FORWARD_DECLARATIONS_HPP_

#include <memory>

#include "vulkan.hpp"

namespace gfx {

class CommandBuffer;
class ComputeBuffer;
class ComputeTexture;
class ComputePipeline;
class DescriptorSetLayout;
class DescriptorSet;
class FrameBuffer;
class GraphicsPipeline;
class Image;
class ImageView;
class Instance;
class LogicalDevice;
class Pipeline;
class PhysicalDevice;
class ProgrammableShader;
class RayTracingPipeline;
class Sampler;
class Surface;
class Queue;

typedef std::shared_ptr<CommandBuffer> CommandBufferPtr;
typedef std::shared_ptr<ComputeBuffer> ComputeBufferPtr;
typedef std::shared_ptr<ComputeTexture> ComputeTexturePtr;
typedef std::shared_ptr<ComputePipeline> ComputePipelinePtr;
typedef std::shared_ptr<DescriptorSetLayout> DescriptorSetLayoutPtr;
typedef std::shared_ptr<DescriptorSet> DescriptorSetPtr;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
typedef std::shared_ptr<GraphicsPipeline> GraphicsPipelinePtr;
typedef std::shared_ptr<Image> ImagePtr;
typedef std::shared_ptr<ImageView> ImageViewPtr;
typedef std::shared_ptr<Instance> InstancePtr;
typedef std::shared_ptr<LogicalDevice> LogicalDevicePtr;
typedef std::shared_ptr<Pipeline> PipelinePtr;
typedef std::shared_ptr<PhysicalDevice> PhysicalDevicePtr;
typedef std::shared_ptr<ProgrammableShader> ProgrammableShaderPtr;
typedef std::shared_ptr<RayTracingPipeline> RayTracingPipelinePtr;
typedef std::shared_ptr<Sampler> SamplerPtr;
typedef std::shared_ptr<Surface> SurfacePtr;

typedef std::unique_ptr<Instance> InstanceUnique;

typedef std::weak_ptr<LogicalDevice> LogicalDeviceWeakPtr;
typedef std::weak_ptr<ComputeTexture> ComputeTextureWeakPtr;
typedef std::weak_ptr<Pipeline> PipelineWeakPtr;
typedef std::weak_ptr<RayTracingPipeline> RayTracingPipelineWeakPtr;

}  // namespace gfx

#endif  // DALI_GRAPHICS_VK_WRAPPERS_FORWARD_DECLARATIONS_HPP_