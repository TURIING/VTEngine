//
// Created by turiing on 2025/1/5.
//
#include "core/RHI/RHIDepthResource.h"

#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHIImage.h"

RHIDepthResource::RHIDepthResource(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, Size size): m_pDevice(device) {
    RHIImageCreateInfo createInfo {
        .size = size,
        .format = m_pDevice->GetDepthFormatDetail(),
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT,
        .memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    };
    m_pImage = std::make_unique<RHIImage>(m_pDevice, commandPool, createInfo);
    m_pImage->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkImageView RHIDepthResource::GetImageView() const {
    return m_pImage->GetImageViewHandle();
}
