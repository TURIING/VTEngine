//
// Created by turiing on 2025/1/7.
//

#ifndef MODELLOADINSTANCE_H
#define MODELLOADINSTANCE_H

#include "common/common.h"
#include "instance/InstanceBase.h"

class RHIDescriptorPool;
class RHISemaphore;
class RHIFence;
class RHICommandBuffer;
class RHIFrameBuffer;
class ForwardPipeLine;
class RHIDescriptorSetLayout;
class RHIRenderPass;
class RHIDepthResource;
class RHISurface;
class RHIInstance;

class ModelLoadInstance: public InstanceBase {
public:
    explicit ModelLoadInstance(const PlatformWindowInfo &info);

private:
    std::shared_ptr<RHIDepthResource> m_pDepthResource;
    std::shared_ptr<RHIRenderPass> m_pRenderPass;
    std::shared_ptr<RHIDescriptorSetLayout> m_pDescriptorSetLayout;
    std::shared_ptr<ForwardPipeLine> m_pForwardPipe;
    std::vector<std::shared_ptr<RHIFrameBuffer>> m_vecFrameBuffers;
    std::shared_ptr<RHICommandBuffer> m_pCommandBuffer;
    std::vector<std::shared_ptr<RHIFence>> m_vecInFlightFence;
    std::vector<std::shared_ptr<RHISemaphore>> m_vecRenderFinishedSemaphore;
    std::vector<std::shared_ptr<RHISemaphore>> m_vecImageAvailableSemaphore;
    std::shared_ptr<RHIDescriptorPool> m_pDescriptorPool;
};
#endif //MODELLOADINSTANCE_H
