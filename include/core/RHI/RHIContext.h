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
#include "core/common/Vertex.h"

class RHIDescriptorSet;
class RHIDescriptorSetLayout;
class RHIDescriptorPool;
class RHIUniformBuffer;
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
class RHIVertexBuffer;
class RHIIndexBuffer;

struct GlobalUniformObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class RHIContext {
public:
    explicit RHIContext(const PlatformWindowInfo &info);
    void Render();
    void Resize(Size size);

private:
    bool prepareFrame(uint32_t &imageIndex);
    void createSyncObject();
    void cleanSwapChain();
    void updateUniformBuffer() const;

private:
    std::shared_ptr<RHIInstance> m_pInstance;
    std::shared_ptr<RHISurface> m_pSurface;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHISwapChain> m_pSwapChain;
    std::shared_ptr<RHIRenderPass> m_pRenderPass;
    std::shared_ptr<ForwardPipeLine> m_pForwardPipeLine;
    std::vector<std::shared_ptr<RHIFrameBuffer>> m_vecFrameBuffer;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    std::shared_ptr<RHIVertexBuffer> m_pVertexBuffer;
    std::shared_ptr<RHIIndexBuffer> m_pIndexBuffer;
    std::vector<std::shared_ptr<RHIUniformBuffer>> m_vecUniformBuffer;
    std::shared_ptr<RHIDescriptorPool> m_pDescriptorPool;
    std::shared_ptr<RHIDescriptorSetLayout> m_pDescriptorSetLayout;
    std::vector<std::shared_ptr<RHIDescriptorSet>> m_vecDescriptorSet;
    std::shared_ptr<RHICommandBuffer> m_pCommandBuffer;
    std::vector<std::shared_ptr<RHIFence>> m_vecInFlightFence;
    std::vector<std::shared_ptr<RHISemaphore>> m_vecImageAvailableSemaphore;
    std::vector<std::shared_ptr<RHISemaphore>> m_vecRenderFinishedSemaphore;
    Size m_size;
    uint32_t m_currentFrameIndex = 0;
    const std::vector<Vertex> m_vecVertices = {
            { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
            { { 0.5f,  -0.5f, 0.0f }, { 0.0, 1.0, 0.0 } },
            { { 0.5f,  0.5f, 0.0f }, { 0.0, 0.0, 1.0 } },
            { { -0.5f, 0.5f, 0.0f }, { 1.0, 1.0, 1.0} },
    };
    const std::vector<uint32_t> m_vecIndices = { 0, 1, 2, 2, 3, 0 };

};

#endif //RHICONTEXT_H
