#include "compute_texture.hpp"
#include "vk_wrappers/utils/image_utils.hpp"
#include "vk_wrappers/command_buffer.hpp"

namespace gfx {

ComputeTexture::ComputeTexture(std::shared_ptr<LogicalDevice> device, const ImageUtils::Data& image_data)
: device_(device) {
    image_ = image_data.image;
    memory_ = image_data.memory;
    format_ = image_data.format;
    layout_ = image_data.layout;
    width_ = image_data.extent.width;
    height_ = image_data.extent.height;
    image_view_ = ImageUtils::createImageView(device, image_data);
    sampler_ = Sampler::create(device, true, false);
}


ComputeTexture::ComputeTexture(std::shared_ptr<LogicalDevice> device, 
                               uint32_t width, 
                               uint32_t height, 
                               uint32_t channels,
                               const uint8_t* pixels,
                               bool lerp, bool unorm)
: device_(device)
, width_(width)
, height_(height)
, channels_(channels)
, channel_size_(4) {
    ImageUtils::Data image_data = ImageUtils::create8BitUnormImage(device, width, height, channels, pixels);

    image_ = image_data.image;
    memory_ = image_data.memory;
    format_ = image_data.format;
    layout_ = image_data.layout;
    image_view_ = ImageUtils::createImageView(device, image_data);
    sampler_ = Sampler::create(device, lerp && !unorm, unorm);
}

ComputeTexture::ComputeTexture(std::shared_ptr<LogicalDevice> device, 
                               uint32_t width, 
                               uint32_t height, 
                               const half* pixels,
                               bool lerp, bool unorm)
: device_(device)
, width_(width)
, height_(height)
, channel_size_(2) {
    CXL_VLOG(3) << "ComputeTexture: Creating HDR Image!";
    ImageUtils::Data image_data = ImageUtils::createHDRImage(device, width, height, pixels);
    image_ = image_data.image;
    memory_ = image_data.memory;
    format_ = image_data.format;
    layout_ = image_data.layout;
    image_view_ = ImageUtils::createImageView(device, image_data);
    sampler_ = Sampler::create(device, lerp && !unorm, unorm);
}

ComputeTexture::ComputeTexture(vk::ImageView view, 
                               vk::Image image, 
                               vk::ImageLayout layout, 
                               vk::Format format,
                               SamplerPtr sampler,
                               uint32_t width,
                               uint32_t height) 
: image_view_(view)
, image_(image)
, layout_(layout)
, format_(format)
, sampler_(std::move(sampler))
, width_(width)
, height_(height){}

ComputeTexture::~ComputeTexture() {
    auto device = device_.lock();
    if (device) {
        device->vk().destroyImageView(image_view_);
        device->vk().destroyImage(image_);
        device->vk().freeMemory(memory_);
    }
}

void ComputeTexture::transitionImageLayout(CommandBuffer& command_buffer, vk::ImageLayout layout) {

    command_buffer.transitionImageLayout(image_, format_, layout_, layout);
    layout_ = layout;
}

} // gfx