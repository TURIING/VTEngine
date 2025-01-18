//
// Created by turiing on 2025/1/4.
//
#include "core/RHI/RHITexture.h"

#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHIImage.h"
#include "core/RHI/RHISampler.h"
#include "core/RHI/RHISingleTimeCommandBuffer.h"
#include "utility/File.h"

RHITexture::RHITexture(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, File &&file): m_pDevice(device), m_pCommandPool(commandPool) {
    const auto imageInfo = file.GetImageData();

    // 先将图像数据拷贝到暂存缓存
    VkDeviceSize imageSize = imageInfo.size.width * imageInfo.size.height * 4;
    VkBuffer stageBuffer;
    VkDeviceMemory stageBufferMemory;
    m_pDevice->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stageBuffer, stageBufferMemory);

    void *data = nullptr;
    vkMapMemory(m_pDevice->GetHandle(), stageBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, imageInfo.data, static_cast<size_t>(imageSize));
    vkUnmapMemory(m_pDevice->GetHandle(), stageBufferMemory);

    RHIImageCreateInfo imageCreateInfo = {
        .size = imageInfo.size,
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT,
        .memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    };
    m_pImage = std::make_shared<RHIImage>(m_pDevice, commandPool, imageCreateInfo);
    m_pImage->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    this->copyBufferToImage(stageBuffer, imageInfo.size);
    m_pImage->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(m_pDevice->GetHandle(), stageBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetHandle(), stageBufferMemory, nullptr);

    m_pSampler = std::make_shared<RHISampler>(m_pDevice);

    m_imageInfo = VkDescriptorImageInfo {
        .sampler = m_pSampler->GetHandle(),
        .imageView = m_pImage->GetImageViewHandle(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    LOG_INFO("RHI texture created");
}

RHITexture::~RHITexture() {
}

void RHITexture::copyBufferToImage(const VkBuffer buffer, Size size) {
    const auto device = m_pDevice.get();
    const auto commandBuffer = std::make_unique<RHISingleTimeCommandBuffer>(device, m_pCommandPool);

    VkBufferImageCopy region = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .imageOffset = {
            .x = 0,
            .y = 0,
            .z = 0,
        },
        .imageExtent = {
            .width = size.width,
            .height = size.height,
            .depth = 1,
        },
    };
    commandBuffer->CopyImage(buffer, m_pImage->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}
