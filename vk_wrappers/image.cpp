#include "vk_wrappers/image.hpp"

namespace gfx {

bool Image::Info::operator(const Info& info) {
    return format == other.format && width == other.width && height == other.height &&
           sample_count == other.sample_count && usage == other.usage &&
           memory_flags == other.memory_flags && tiling == other.tiling &&
           is_mutable == other.is_mutable && is_external == other.is_external;
}


std::pair<vk::PipelineStageFlags, vk::AccessFlag> 
Image::stageAndAccessFlagsForLayout(const vk::ImageLayout& layout) {
    switch(layout) {
        case vk::ImageLayout::eUndefined: {
            return { vk::AccessFlags(), 
                     vk::PipelineStageFlagBits::eTopOfPipe };
        }   
        case vk::ImageLayout::eTransferSrcOptimal: {
            return { vk::AccessFlags(), 
                     vk::PipelineStageFlagBits::eTransfer };
        }
        case vk::ImageLayout::eTransferDstOptimal: {
            return { vk::AccessFlagBits::eTransferWrite,
                     vk::PipelineStageFlagBits::eTransfer };
        }
        case vk::ImageLayout::eShaderReadOnlyOptimal: {
            return { vk::AccessFlagBits::eShaderRead,
                     vk::PipelineStageFlagBits::eComputeShader };
        }
        case vk::ImageLayout::eColorAttachmentOptimal: {
            return { vk::AccessFlagBits::eColorAttachmentRead  |
                     vk::AccessFlagBits::eColorAttachmentWrite |
                     vk::AccessFlagBits::eShaderRead, 
                     vk::PipelineStageFlagBits::eAllGraphics };
        }
        case vk::ImageLayout::ePresentSrcKHR: {
            return { vk::AccessFlagBits::eColorAttachmentRead  |
                     vk::AccessFlagBits::eColorAttachmentWrite |
                     vk::AccessFlagBits::eShaderRead, 
                     vk::PipelineStageFlagBits::eAllGraphics };
        }
        default: 
            throw std::invalid_argument("Unsupported layout!" + vk::to_string(layout));
            return { vk::AccessFlagBits(), vk::PipelineStageFlagBits::eTopOfPipe };
    }
}


} // namespace gfx
