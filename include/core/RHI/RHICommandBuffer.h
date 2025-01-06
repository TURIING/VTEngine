/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 17:18
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHICOMMANDBUFFER_H
#define RHICOMMANDBUFFER_H

#include "common/common.h"

class RHIFence;
class RHISemaphore;
class ForwardPipeLine;
class RHIFrameBuffer;
class RHIRenderPass;
class RHICommandPool;
class RHIDevice;
class RHIVertexBuffer;
class RHIIndexBuffer;

class RHICommandBuffer {
public:
    RHICommandBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, uint32_t count);
    ~RHICommandBuffer();
    void Reset(uint32_t index) const;
    void BeginRecord(uint32_t currentframeIndex) const;
    void EndRecord(uint32_t currentframeIndex) const;
    void BeginRenderPass(const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIFrameBuffer>& framebuffer, uint32_t currentFrame, Size size) const;
    void EndRenderPass(uint32_t currentFrameIndex) const;
    void BindPineLine(const std::shared_ptr<ForwardPipeLine>& pineLine, uint32_t currentFrameIndex) const;
    void BindVertexBuffer(uint32_t currentFrameIndex, const std::shared_ptr<RHIVertexBuffer> &buffer, VkDeviceSize *offset, uint32_t firstBindingIndex, uint32_t bindingCount) const;
    void BindIndexBuffer(uint32_t currentFrameIndex, const std::shared_ptr<RHIIndexBuffer> &indexBuffer, VkDeviceSize offset, VkIndexType indexType) const;
    void BindDescriptorSets(uint32_t currentFrameIndex, VkPipelineBindPoint bindPoint, VkPipelineLayout layout, const std::vector<VkDescriptorSet> &descriptorSets, uint32_t firstSetIndex) const;
    void SetViewport(uint32_t currentFrameIndex, uint32_t firstViewportIndex, uint32_t viewportCount, const VkViewport &viewport) const;
    void SetScissor(uint32_t currentFrameIndex, uint32_t firstScissorIndex, uint32_t scissorCount, const VkRect2D &scissor) const;
    void Draw(uint32_t currentFrameIndex, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertexIndex, uint32_t firstInstanceIndex) const;
    void DrawIndex(uint32_t currentFrameIndex, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance) const;
    void Submit(uint32_t currentFrameIndex, const std::shared_ptr<RHISemaphore> &waitSemaphore, const std::shared_ptr<RHISemaphore> &signalSemaphore, const std::shared_ptr<RHIFence> &inFightFence) const;
    void PushConstants(uint32_t currentFrameIndex, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void *data) const;

private:
    std::vector<VkCommandBuffer> m_vecCommandBuffer;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
};


#endif //RHICOMMANDBUFFER_H
