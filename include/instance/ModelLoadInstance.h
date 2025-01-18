//
// Created by turiing on 2025/1/7.
//

#ifndef MODELLOADINSTANCE_H
#define MODELLOADINSTANCE_H

#include "common/common.h"
#include "instance/InstanceBase.h"

class RHIDescriptorSet;
class RHIUniformBuffer;
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
    void OnRender() override;

private:
    bool prepareFrame(uint32_t &imageIndex);
    void updateUniformBuffer() const;
    void cleanSwapChain() override;

private:
    std::shared_ptr<RHIDescriptorSetLayout> m_pDescriptorSetLayout;
    std::shared_ptr<ForwardPipeLine> m_pForwardPipeLine;
    std::shared_ptr<RHICommandBuffer> m_pCommandBuffer;
    std::vector<std::shared_ptr<RHIFence>> m_vecInFlightFence;
    std::vector<std::shared_ptr<RHISemaphore>> m_vecRenderFinishedSemaphore;
    std::vector<std::shared_ptr<RHISemaphore>> m_vecImageAvailableSemaphore;
    std::shared_ptr<RHIDescriptorPool> m_pDescriptorPool;
    std::vector<std::shared_ptr<RHIUniformBuffer>> m_vecUniformBuffer;
    std::vector<std::shared_ptr<RHIDescriptorSet>> m_vecDescriptorSet;
    uint32_t m_currentFrameIndex = 0;
};
#endif //MODELLOADINSTANCE_H
