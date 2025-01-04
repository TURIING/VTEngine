//
// Created by turiing on 2024/12/28.
//

#ifndef VTENGINE_RHISINGLETIMECOMMANDBUFFER_H
#define VTENGINE_RHISINGLETIMECOMMANDBUFFER_H

#include "common/common.h"

class RHIDevice;
class RHICommandPool;

class RHISingleTimeCommandBuffer {
public:
    explicit RHISingleTimeCommandBuffer(RHIDevice *device, const std::shared_ptr<RHICommandPool> &commandPool);
    ~RHISingleTimeCommandBuffer();
    [[nodiscard]] VkCommandBuffer GetHandle() const { return m_pCommandBuffer; }
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    void CopyImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout imageLayout, uint32_t regionCount, const VkBufferImageCopy *regions) const;
    void PipelineBarrier(VkPipelineStageFlags srcStageFlags, VkPipelineStageFlags dstStageFlags, VkImageMemoryBarrier &barrier) const;

private:
    RHIDevice *m_pDevice = nullptr;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    VkCommandBuffer m_pCommandBuffer = nullptr;
};

#endif //VTENGINE_RHISINGLETIMECOMMANDBUFFER_H
