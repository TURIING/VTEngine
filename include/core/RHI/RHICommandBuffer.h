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

class RHICommandBuffer {
public:
    RHICommandBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, uint32_t count);
    ~RHICommandBuffer();
    void Reset(uint32_t index) const;
    void BeginRecord(uint32_t currentframeIndex);
    void EndRecord(uint32_t currentframeIndex);
    void BeginRenderPass(const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIFrameBuffer>& framebuffer, uint32_t currentFrame, Size size) const;
    void EndRenderPass(uint32_t currentFrameIndex) const;
    void BindPineLine(const std::shared_ptr<ForwardPipeLine>& pineLine, uint32_t currentFrameIndex) const;
    void SetViewport(uint32_t currentFrameIndex, uint32_t firstViewportIndex, uint32_t viewportCount, const VkViewport &viewport) const;
    void SetScissor(uint32_t currentFrameIndex, uint32_t firstScissorIndex, uint32_t scissorCount, const VkRect2D &scissor) const;
    void Draw(uint32_t currentFrameIndex);
    void Submit(uint32_t currentFrameIndex, const std::shared_ptr<RHISemaphore> &waitSemaphore, const std::shared_ptr<RHISemaphore> &signalSemaphore, const std::shared_ptr<RHIFence> &inFightFence) const;

private:
    std::vector<VkCommandBuffer> m_vecCommandBuffer;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
};


#endif //RHICOMMANDBUFFER_H
