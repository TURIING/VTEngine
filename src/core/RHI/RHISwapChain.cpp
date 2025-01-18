/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/8 16:36
* @version: 1.0
* @description: 
********************************************************************************/

#include "core/RHI/RHISwapChain.h"

#include <core/RHI/RHIDevice.h>
#include <core/RHI/RHISemaphore.h>
#include <core/RHI/RHISurface.h>

SwapChainSupportDetails SwapChainSupportDetails::GetSwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if(formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if(presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

RHISwapChain::RHISwapChain(const std::shared_ptr<RHIInstance> &instance, const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHISurface> &surface, const Size &size)
    : m_pInstance(instance), m_pDevice(device), m_size(size){
    m_swapChainSupportDetails = SwapChainSupportDetails::GetSwapChainSupportDetails(m_pDevice->GetPhysicalDeviceHandle(), surface->GetHandle());
    m_pSwapChainSurfaceFormat = this->chooseSwapSurfaceFormat();
    VkPresentModeKHR presentMode = this->chooseSwapPresentMode();
    VkExtent2D swapExtent = this->getSwapChainExtent();
    uint32_t imageCount = this->getSwapChainImageCount();

    VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface->GetHandle(),
        .minImageCount = imageCount,
        .imageFormat = m_pSwapChainSurfaceFormat.format,
        .imageColorSpace = m_pSwapChainSurfaceFormat.colorSpace,
        .imageExtent = swapExtent,
        .imageArrayLayers = 1,                                                                  // 指定每个图像所包含的层次。通常，来说它的值为1, 但对于VR相关的应用程序来说，会使用更多的层次
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                                       // 我们在图像上进行绘制操作，也就是将图像作为一个颜色附着来使用
    };

    // 指定在多个队列族使用交换链图像的方式
    QueueFamilyIndices indices = QueueFamilyIndices::GetQueueFamilyIndices(m_pDevice->GetPhysicalDeviceHandle(), surface->GetHandle());
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    if(indices.graphicsFamily != indices.presentFamily) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapChainCreateInfo.preTransform = m_swapChainSupportDetails.capabilities.currentTransform;         // 为交换链中的图像指定一个固定的变换操作,比如顺时针旋转90度或是水平翻转
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                             // 用于指定alpha通道是否被用来和窗口系统中的其它窗口进行混合操作。
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;                                                              // 设置为VK TRUE表示我们不关心被窗口系统中的其它窗口遮挡的像素的颜色
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    CALL_VK(vkCreateSwapchainKHR(m_pDevice->GetHandle(), &swapChainCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Created SwapChain");

    this->createSwapChainImagesAndViews();
}

RHISwapChain::~RHISwapChain() {
    for(const auto pImageView: m_vecSwapChainImageViews) {
        vkDestroyImageView(m_pDevice->GetHandle(), pImageView, nullptr);
    }
    vkDestroySwapchainKHR(m_pDevice->GetHandle(), m_pHandle, nullptr);
}

VkImage RHISwapChain::GetImage(uint32_t index) const {
    LOG_ASSERT(index >= 0 && index < m_vecSwapChainImages.size());
    return m_vecSwapChainImages[index];
}

VkImageView RHISwapChain::GetImageView(uint32_t index) const {
    LOG_ASSERT(index >= 0 && index < m_vecSwapChainImageViews.size());
    return m_vecSwapChainImageViews[index];
}

VkResult RHISwapChain::AcquireNextImage(const std::shared_ptr<RHISemaphore> &semaphore, uint32_t &imageIndex) const {
    return vkAcquireNextImageKHR(m_pDevice->GetHandle(), m_pHandle, UINT64_MAX, semaphore->GetHandle(), nullptr, &imageIndex);
}

// 选择合适的表面格式
VkSurfaceFormatKHR RHISwapChain::chooseSwapSurfaceFormat() const {
    for(const auto &availableFormat : m_swapChainSupportDetails.formats) {
        // 1.表示我们以B，G，R和A的顺序,每个颜色通道用8位无符号整型数表示，总共每像素使用32位表示
        // 2.表示SRGB颜色空间是否被支持
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return m_swapChainSupportDetails.formats[0];
}

// 选择合适的呈现模式
VkPresentModeKHR RHISwapChain::chooseSwapPresentMode() const {
    for(const auto &availablePresentMode : m_swapChainSupportDetails.presentModes) {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RHISwapChain::getSwapChainExtent() {
    m_size = {
        .width = m_swapChainSupportDetails.capabilities.currentExtent.width,
        .height = m_swapChainSupportDetails.capabilities.currentExtent.height,
    };
    return m_swapChainSupportDetails.capabilities.currentExtent;
}

uint32_t RHISwapChain::getSwapChainImageCount() const {
    uint32_t imageCount = m_swapChainSupportDetails.capabilities.minImageCount + 1;
    if(m_swapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount > m_swapChainSupportDetails.capabilities.maxImageCount) {
        imageCount = m_swapChainSupportDetails.capabilities.maxImageCount;
    }
    return imageCount;
}

void RHISwapChain::createSwapChainImagesAndViews() {
    uint32_t imageCount = 0;
    // images
    vkGetSwapchainImagesKHR(m_pDevice->GetHandle(), m_pHandle, &imageCount, nullptr);
    m_vecSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_pDevice->GetHandle(), m_pHandle, &imageCount, m_vecSwapChainImages.data());

    // views
    m_vecSwapChainImageViews.resize(imageCount);
    for(uint32_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo imageViewCreateInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = m_vecSwapChainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = m_pSwapChainSurfaceFormat.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };
        CALL_VK(vkCreateImageView(m_pDevice->GetHandle(), &imageViewCreateInfo, nullptr, &m_vecSwapChainImageViews[i]));
    }
    LOG_INFO("Created SwapChain images");
    LOG_INFO("SwapChain image size: ({}, {}), image count: {}", m_size.width, m_size.height, imageCount);
}
