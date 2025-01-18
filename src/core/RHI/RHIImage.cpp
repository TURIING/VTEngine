//
// Created by turiing on 2025/1/4.
//
#include "core/RHI/RHIImage.h"

#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHISingleTimeCommandBuffer.h"

RHIImage::RHIImage(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, const RHIImageCreateInfo& info)
    : m_pDevice(device), m_pCommandPool(commandPool), m_mipLevels(info.mipLevels), m_imageType(info.imageType) {
    VkImageCreateInfo imageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .extent = {
            .width = info.size.width,
            .height = info.size.height,
            .depth = 1,
        },
        .mipLevels = info.mipLevels,
        .arrayLayers = 1,
        .format = info.format,
        .tiling = info.tiling,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .usage = info.usage,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    if(info.imageType == ImageType::Texture3D) {
        imageCreateInfo.arrayLayers = 6;
        imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }
    CALL_VK(vkCreateImage(m_pDevice->GetHandle(), &imageCreateInfo, nullptr, &m_pImage));


    // 分配图像内存
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(m_pDevice->GetHandle(), m_pImage, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = m_pDevice->FindMemoryType(memoryRequirements.memoryTypeBits, info.memoryPropertyFlags),
    };
    CALL_VK(vkAllocateMemory(m_pDevice->GetHandle(), &memoryAllocateInfo, nullptr, &m_pDeviceMemory));

    vkBindImageMemory(m_pDevice->GetHandle(), m_pImage, m_pDeviceMemory, 0);

    // 创建图像视图
    VkImageViewCreateInfo imageViewCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_pImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = info.format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = info.aspectFlags,
            .baseMipLevel = 0,
            .levelCount = info.mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
    };
    if(m_imageType == ImageType::Texture3D) {
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE,
        imageViewCreateInfo.subresourceRange.layerCount = 6;
    }
    CALL_VK(vkCreateImageView(m_pDevice->GetHandle(), &imageViewCreateInfo, nullptr, &m_pImageView));
}

RHIImage::~RHIImage() {
    vkDestroyImage(m_pDevice->GetHandle(), m_pImage, nullptr);
    vkFreeMemory(m_pDevice->GetHandle(), m_pDeviceMemory, nullptr);
    vkDestroyImageView(m_pDevice->GetHandle(), m_pImageView, nullptr);
}

void RHIImage::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectFlags) {
    const auto device = m_pDevice.get();
    auto commandBuffer = std::make_unique<RHISingleTimeCommandBuffer>(device, m_pCommandPool);

    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = oldLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = m_pImage,
        .subresourceRange = {
            .aspectMask = aspectFlags,
            .baseMipLevel = 0,
            .levelCount = m_mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    if(m_imageType == ImageType::Texture3D) {
        barrier.subresourceRange.layerCount = 6;
    }

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else {
        LOG_CRITICAL("RHIImage::transitionImageLayout: Unsupported image layout!");
    }
    commandBuffer->PipelineBarrier(sourceStage, destinationStage, barrier);
}
