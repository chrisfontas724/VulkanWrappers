// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "vk_wrappers/swap_chain.hpp"

#include <map>

#include "logging/logging.hpp"
#include "vk_wrappers/compute_texture.hpp"
#include "vk_wrappers/physical_device.hpp"
#include "vk_wrappers/utils/image_utils.hpp"

namespace gfx {

namespace {

const int MAX_FRAMES_IN_FLIGHT = 2;

vk::PresentModeKHR chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& available_present_modes) {
    vk::PresentModeKHR best_mode = vk::PresentModeKHR::eFifo;
    for (const auto& available_present_mode : available_present_modes) {
        if (available_present_mode == vk::PresentModeKHR::eMailbox) {
            return available_present_mode;
        } else if (available_present_mode == vk::PresentModeKHR::eImmediate) {
            best_mode = available_present_mode;
        }
    }
    return best_mode;
}

vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, uint32_t width,
                              uint32_t height) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        vk::Extent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        return actualExtent;
    }
}

}  // anonymous namespace

vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
        vk::SurfaceFormatKHR result;
        result.format = vk::Format::eB8G8R8A8Unorm;
        result.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        return result;
    }

    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

SwapChain::SwapChain(LogicalDevicePtr logical_device, vk::SurfaceKHR surface, uint32_t width,
                     uint32_t height)
    : logical_device_(logical_device), surface_(surface), current_frame_(0) {
    // Make sure everything is valid.
    CXL_DCHECK(logical_device);
    CXL_DCHECK(surface);
    CXL_DCHECK(width > 0 && height > 0);
    CXL_VLOG(5) << "\n";
    CXL_VLOG(5) << "Swapchain Constructor Verbose:";
    CXL_VLOG(5) << "------------------------------";

    try {
        // Get support details.
        auto physical_device = logical_device->physical_device();
        auto support_details = physical_device->querySwapChainSupport(surface);
        surface_format_ = chooseSwapSurfaceFormat(support_details.formats);
        present_mode_ = chooseSwapPresentMode(support_details.present_modes);
        extent_ = chooseSwapExtent(support_details.capabilities, width, height);
        CXL_VLOG(5) << "    Retrieved surface format, present mode and extent!";
        CXL_VLOG(5) << "        Extent: (" << extent_.width << ", " << extent_.height << ")";

        uint32_t image_count = support_details.capabilities.minImageCount + 1;
        if (support_details.capabilities.maxImageCount > 0 &&
            image_count > support_details.capabilities.maxImageCount) {
            image_count = support_details.capabilities.maxImageCount;
        }
        CXL_VLOG(5) << "    Image count is " << image_count;

        vk::SwapchainCreateInfoKHR create_info(
            {}, surface, image_count, surface_format_.format, surface_format_.colorSpace, extent_,
            1, /*array layers*/
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc |
                vk::ImageUsageFlagBits::eTransferDst);
        CXL_VLOG(5) << "    Initialize create info";

        auto indices = physical_device->queue_family_indices(surface);
        uint32_t queue_family_indices[] = {indices.graphics_family.value(),
                                           indices.present_family.value()};
        CXL_VLOG(5) << "    Queue family indices: " << queue_family_indices[0] << " "
                    << queue_family_indices[1];

        if (indices.graphics_family != indices.present_family) {
            create_info.imageSharingMode = vk::SharingMode::eConcurrent;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        } else {
            create_info.imageSharingMode = vk::SharingMode::eExclusive;
        }

        create_info.preTransform = support_details.capabilities.currentTransform;
        create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        create_info.presentMode = present_mode_;
        CXL_VLOG(5) << "    Set preTransform, compositAlpha and presentMode";

        swap_chain_ = logical_device->vk().createSwapchainKHRUnique(create_info);
        CXL_VLOG(5) << "    Created swap chain!";

        images_ = logical_device->vk().getSwapchainImagesKHR(swap_chain_.get());
        CXL_VLOG(5) << "    Created swap chain images: " << images_.size();

        CXL_VLOG(3) << "SURFACE FORMAT: " << vk::to_string(surface_format_.format);

        uint32_t index = 0;
        for (const auto& image : images_) {
            auto image_view = ImageUtils::createImageView(
                logical_device, image, surface_format_.format, vk::ImageAspectFlagBits::eColor);
            auto sampler = Sampler::create(logical_device);
            auto texture = std::make_shared<ComputeTexture>(
                image_view, images_[index], vk::ImageLayout::ePresentSrcKHR, surface_format_.format,
                std::move(sampler), extent_.width, extent_.height);
            textures_.push_back(texture);
            index++;
        }
        CXL_VLOG(5) << "    Created swap chain image views!";

        image_available_semaphores_ = logical_device->createSemaphores(MAX_FRAMES_IN_FLIGHT);
        in_flight_fences_.resize(MAX_FRAMES_IN_FLIGHT);
        images_in_flight_.resize(images_.size());

        in_flight_fences_ =
            logical_device->createFences(MAX_FRAMES_IN_FLIGHT, vk::FenceCreateFlagBits::eSignaled);
        CXL_VLOG(5) << "    Created fences and semaphores!";

    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    } catch (...) {
        std::cout << "unknown error\n";
        exit(-1);
    }
}

SwapChain::~SwapChain() {
    if (const auto& device = logical_device_.lock()) {
        swap_chain_.reset();
        for (const auto& semaphore : image_available_semaphores_) {
            device->destroy(semaphore);
        }
        for (const auto& fence : in_flight_fences_) {
            device->destroy(fence);
        }
        for (auto& texture : textures_) {
            texture.reset();
        }
    }
}

void SwapChain::beginFrame(RenderFunction callback) {
    const auto& device = logical_device_.lock();
    CXL_CHECK(device);
    CXL_DCHECK(callback);

    // Wait for the current in flight fence.
    device->waitForFence(in_flight_fences_[current_frame_]);

    try {
        image_index_ =
            device->vk()
                .acquireNextImageKHR(swap_chain_.get(), std::numeric_limits<uint64_t>::max(),
                                     image_available_semaphores_[current_frame_], vk::Fence())
                .value;

        if (images_in_flight_[image_index_] != vk::Fence()) {
            device->waitForFence(images_in_flight_[image_index_]);
        }

        images_in_flight_[image_index_] = in_flight_fences_[current_frame_];

        device->vk().resetFences(1, &in_flight_fences_[current_frame_]);

        present(callback(image_available_semaphores_[current_frame_],
                         in_flight_fences_[current_frame_], image_index_, current_frame_));

    } catch (vk::SystemError err) {
        CXL_LOG(WARNING) << err.what();
        throw std::runtime_error("failed to acquire swap chain image!");
    }
}

void SwapChain::present(const std::vector<vk::Semaphore>& semaphores) {
    const auto& device = logical_device_.lock();
    CXL_CHECK(device);

    // Present frame.
    vk::PresentInfoKHR present_info(/*waitSemaphoreCount_*/ semaphores.size(), semaphores.data(),
                                    /*swapChainCount_*/ 1, &swap_chain_.get(),
                                    /*pImageIndices*/ &image_index_);
    device->getQueue(gfx::Queue::Type::kPresent).present(present_info);

    current_frame_ = (current_frame_ + 1) % MAX_FRAMES_IN_FLIGHT;
}

}  // namespace gfx