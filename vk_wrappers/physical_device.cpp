#include "vk_wrappers/physical_device.hpp"
#include "logging/logging.hpp"
#include <set>

namespace gfx {

namespace {

const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    /* VK_NV_RAY_TRACING_EXTENSION_NAME */
};
        
bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device) {
    std::vector<vk::ExtensionProperties> available_extensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());
        
    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }
    return required_extensions.empty();
}
}

PhysicalDevice::PhysicalDevice(vk::PhysicalDevice physical_device)
        : physical_device_(physical_device) {
    queue_family_properties_ = physical_device_.getQueueFamilyProperties();
    layer_properties_ = physical_device_.enumerateDeviceLayerProperties();

    properties_ = physical_device_.getProperties();
    features_ = physical_device_.getFeatures();
}

vk::SampleCountFlagBits PhysicalDevice::maximumMSAA() const {
    vk::SampleCountFlags counts = properties_.limits.framebufferColorSampleCounts;
    if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
    if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
    if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
    if (counts & vk::SampleCountFlagBits::e8)  { return vk::SampleCountFlagBits::e8;  }
    if (counts & vk::SampleCountFlagBits::e4)  { return vk::SampleCountFlagBits::e4;  }
    if (counts & vk::SampleCountFlagBits::e2)  { return vk::SampleCountFlagBits::e2;  }
    return vk::SampleCountFlagBits::e1;
}


void PhysicalDevice::printDiagnostics() {

    CXL_LOG(INFO) << "Device: " << properties_.deviceName;
    CXL_LOG(INFO) << "    1. ID: " << properties_.deviceID;
    CXL_LOG(INFO) << "    2. Type: " << vk::to_string(properties_.deviceType);
    CXL_LOG(INFO) << "    3. Vendor: " << properties_.vendorID;
    CXL_LOG(INFO) << "    4. API Version: " << properties_.apiVersion;
    CXL_LOG(INFO) << "    5. Driver Version: " << properties_.driverVersion;
    CXL_LOG(INFO) << "    6. Limits: ";
    CXL_LOG(INFO) << "        a. Max Image Dimensions 2D: "  << properties_.limits.maxImageDimension2D;
    CXL_LOG(INFO) << "        b. Max Storage Buffer Range: "  << properties_.limits.maxStorageBufferRange;
    CXL_LOG(INFO) << "        c. Max Compute Work Group Invocations: "  << properties_.limits.maxComputeWorkGroupInvocations;
    CXL_LOG(INFO) << "        d. Max Compute Work Group Size: "  << properties_.limits.maxComputeWorkGroupSize[0];
    CXL_LOG(INFO) << "    7. Features: ";
    CXL_LOG(INFO) << "        a. Has Geometry Shaders: " << (features_.geometryShader ? "yes" : "no");
    CXL_LOG(INFO) << "\n\n";
}

uint32_t PhysicalDevice::performanceScore(const vk::SurfaceKHR& surface) const {
    CXL_VLOG(5) << "Checking performance score for " << name();
    uint32_t score = 0;

    // TODO: Figure out which is actually better. Seems like the integrated GPU is performing
    // better, but online guides say discrete GPUs are better...so this needs more research.
    // For now, will give a higher weight to the integrated GPU.    
    if (properties_.deviceType == vk::PhysicalDeviceType::eIntegratedGpu) {
        CXL_VLOG(5) << "    Is an integrated gpu!";
        score += 1000;
    } else if (properties_.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
        CXL_VLOG(5) << "    Is a discrete gpu!";
        score += 1500;
    }
    
    // Maximum possible size of textures affects graphics quality
    score += properties_.limits.maxImageDimension2D;

    QueueFamilyIndices indices = findQueueFamilies(surface);
    CXL_VLOG(5) << "    Acquired queue families";

    bool extensions_supported = checkDeviceExtensionSupport(physical_device_);
    CXL_VLOG(5) << "    Supports Extensions: " << extensions_supported;

    bool swap_chain_adequate = false;
    if (extensions_supported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(surface);
        swap_chain_adequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
        CXL_VLOG(5) << "    Swapchain adequate:  " << swap_chain_adequate;
    }

    CXL_VLOG(5) << "Final score is " << score;

    return indices.isComplete() &&
           extensions_supported &&
           swap_chain_adequate &&
           features_.samplerAnisotropy ? score : 0;
}

PhysicalDevice::QueueFamilyIndices PhysicalDevice::findQueueFamilies(const vk::SurfaceKHR& surface) const{
    std::vector<vk::QueueFamilyProperties> queue_families = physical_device_.getQueueFamilyProperties();
        
    int32_t i = 0;
    QueueFamilyIndices indices;
    for (const auto& queue_family : queue_families) {
        if (queue_family.queueCount > 0 && queue_family.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphics_family = i;
        }

        if (queue_family.queueCount > 0 && queue_family.queueFlags & vk::QueueFlagBits::eCompute) {
            indices.compute_family = i;
        }

        if (queue_family.queueCount > 0 && queue_family.queueFlags & vk::QueueFlagBits::eTransfer) {
            indices.transfer_family = i;
        }
            
        vk::Bool32 present_support = false;
        physical_device_.getSurfaceSupportKHR(i, surface, &present_support);
        if (queue_family.queueCount > 0 && present_support) {
            indices.present_family = i;
        }
            
        if (indices.isComplete()) {
            break;
        }
            
        i++;
    }
    return indices;
}


uint32_t PhysicalDevice::findMemoryType(uint32_t type_filter, vk::MemoryPropertyFlags mem_properties) {
    vk::PhysicalDeviceMemoryProperties test_properties = physical_device_.getMemoryProperties();
    for (uint32_t i = 0; i < test_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) &&
            (test_properties.memoryTypes[i].propertyFlags & mem_properties) == mem_properties) {
            return i;
        }
    }
        
    throw std::runtime_error("failed to find suitable memory type!");
}

PhysicalDevice::SwapChainSupportDetails PhysicalDevice::querySwapChainSupport(const vk::SurfaceKHR& surface) const {

    SwapChainSupportDetails details;
    details.capabilities = physical_device_.getSurfaceCapabilitiesKHR(surface);
    details.formats = physical_device_.getSurfaceFormatsKHR(surface);
    details.present_modes = physical_device_.getSurfacePresentModesKHR(surface);
    return details;
}

} // gfx
