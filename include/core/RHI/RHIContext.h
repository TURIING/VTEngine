/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/9 20:32
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHICONTEXT_H
#define RHICONTEXT_H

#include "common/common.h"

class RHIFence;
class RHISemaphore;
class RHICommandBuffer;
class RHICommandPool;
class RHIFrameBuffer;
class ForwardPipeLine;
class RHIRenderPass;
class RHISurface;
class RHISwapChain;
class RHIDevice;
class RHIInstance;

class RHIContext {
public:
    explicit RHIContext(const PlatformWindowInfo &info);
    void Render() const;

private:
    bool prepareFrame(uint32_t &imageIndex) const;

private:
    std::shared_ptr<RHIInstance> m_pInstance;
    std::shared_ptr<RHISurface> m_pSurface;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHISwapChain> m_pSwapChain;
    std::shared_ptr<RHIRenderPass> m_pRenderPass;
    std::shared_ptr<ForwardPipeLine> m_pForwardPipeLine;
    std::vector<std::shared_ptr<RHIFrameBuffer>> m_vecFrameBuffer;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    std::shared_ptr<RHICommandBuffer> m_pCommandBuffer;
    std::shared_ptr<RHIFence> m_pInFlightFence;
    std::shared_ptr<RHISemaphore> m_pImageAvailableSemaphore;
    std::shared_ptr<RHISemaphore> m_pRenderFinishedSemaphore;
};

#endif //RHICONTEXT_H
