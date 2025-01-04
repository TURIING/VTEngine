//
// Created by turiing on 2024/12/28.
//

#include "core/RHI/RHISingleTimeCommandBuffer.h"
#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHICommandPool.h"

RHISingleTimeCommandBuffer::RHISingleTimeCommandBuffer(RHIDevice *device, const std::shared_ptr<RHICommandPool> &commandPool): m_pDevice(device), m_pCommandPool(commandPool) {
    VkCommandBufferAllocateInfo allocateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_pCommandPool->GetHandle(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    CALL_VK(vkAllocateCommandBuffers(m_pDevice->GetLogicalDeviceHandle(), &allocateInfo, &m_pCommandBuffer));

    VkCommandBufferBeginInfo beginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    vkBeginCommandBuffer(m_pCommandBuffer, &beginInfo);
}

RHISingleTimeCommandBuffer::~RHISingleTimeCommandBuffer() {
    vkEndCommandBuffer(m_pCommandBuffer);

    VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_pCommandBuffer
    };
    vkQueueSubmit(m_pDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_pDevice->GetGraphicsQueue());

    vkFreeCommandBuffers(m_pDevice->GetLogicalDeviceHandle(), m_pCommandPool->GetHandle(), 1, &m_pCommandBuffer);
}

void RHISingleTimeCommandBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const {
    const VkBufferCopy copyRegion {
        .size = size,
    };
    vkCmdCopyBuffer(m_pCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
}

void RHISingleTimeCommandBuffer::CopyImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout imageLayout, uint32_t regionCount, const VkBufferImageCopy* regions) const {
    vkCmdCopyBufferToImage(m_pCommandBuffer, srcBuffer, dstImage, imageLayout, regionCount, regions);
}

void RHISingleTimeCommandBuffer::PipelineBarrier(VkPipelineStageFlags srcStageFlags, VkPipelineStageFlags dstStageFlags, VkImageMemoryBarrier& barrier) const {
    vkCmdPipelineBarrier(m_pCommandBuffer, srcStageFlags, dstStageFlags, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}
