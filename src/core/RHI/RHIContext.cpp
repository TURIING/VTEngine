/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/9 20:32
* @version: 1.0
* @description: 
********************************************************************************/

#include "core/RHI/RHIContext.h"

#include <core/pipeline/ForwardPipeLine.h>
#include <core/RHI/RHICommandBuffer.h>
#include <core/RHI/RHICommandPool.h>
#include <core/RHI/RHIDevice.h>
#include <core/RHI/RHIFence.h>
#include <core/RHI/rhiframebuffer.h>
#include <core/pass/ForwardPass.h>
#include <core/RHI/RHISurface.h>
#include <core/RHI/RHISwapChain.h>
#include <core/RHI/RHIInstance.h>
#include <core/RHI/RHISemaphore.h>

constexpr int MAX_FRAME_IN_FLIGHT = 2;

RHIContext::RHIContext(const PlatformWindowInfo &info): m_size(info.size) {
    m_pInstance = std::make_shared<RHIInstance>();
    m_pSurface = std::make_shared<RHISurface>(m_pInstance, info.handle);
    m_pDevice = std::make_shared<RHIDevice>(m_pInstance, m_pSurface);
    m_pSwapChain = std::make_shared<RHISwapChain>(m_pInstance, m_pDevice, m_pSurface, info.size);
    m_pRenderPass = std::make_shared<ForwardPass>(m_pDevice, m_pSwapChain->GetColorFormat());
    m_pForwardPipeLine = std::make_shared<ForwardPipeLine>(m_pDevice, m_pRenderPass);

    const auto size = m_pSwapChain->GetSize();
    for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
        m_vecFrameBuffer.emplace_back(std::make_shared<RHIFrameBuffer>(m_pDevice, m_pRenderPass, m_pSwapChain->GetImageView(i), size));
    }

    const auto queueFamilyIndices = QueueFamilyIndices::GetQueueFamilyIndices(m_pDevice->GetPhysicalDeviceHandle(), m_pSurface->GetHandle());
    m_pCommandPool = std::make_shared<RHICommandPool>(m_pDevice, queueFamilyIndices.graphicsFamily.value());

    m_pCommandBuffer = std::make_shared<RHICommandBuffer>(m_pDevice, m_pCommandPool, MAX_FRAME_IN_FLIGHT);

    this->createSyncObject();
}

void RHIContext::Render() {
    uint32_t imageIndex = 0;
    if(!prepareFrame(imageIndex)) return;

    m_pCommandBuffer->Reset(m_currentFrameIndex);
    m_pCommandBuffer->BeginRecord(m_currentFrameIndex);
    m_pCommandBuffer->BeginRenderPass(m_pRenderPass, m_vecFrameBuffer[imageIndex], m_currentFrameIndex, m_pSwapChain->GetSize());
    m_pCommandBuffer->BindPineLine(m_pForwardPipeLine, m_currentFrameIndex);

    const auto size = m_pSwapChain->GetSize();
    const VkViewport viewport = {
        .x = 0,
        .y = 0,
        .width = static_cast<float>(size.width),
        .height = static_cast<float>(size.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    m_pCommandBuffer->SetViewport(m_currentFrameIndex, 0, 1, viewport);

    const VkRect2D scissor = {
        .offset = {0, 0},
        .extent = { size.width, size.height }
    };
    m_pCommandBuffer->SetScissor(m_currentFrameIndex, 0, 1, scissor);
    m_pCommandBuffer->Draw(m_currentFrameIndex);
    m_pCommandBuffer->EndRenderPass(m_currentFrameIndex);
    m_pCommandBuffer->EndRecord(m_currentFrameIndex);

    m_pCommandBuffer->Submit(m_currentFrameIndex, m_vecImageAvailableSemaphore[m_currentFrameIndex], m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_vecInFlightFence[m_currentFrameIndex]);
    m_pDevice->Present(m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_pSwapChain, imageIndex);

    m_currentFrameIndex = (m_currentFrameIndex + 1) % MAX_FRAME_IN_FLIGHT;
}

void RHIContext::Resize(Size size) const {
}

bool RHIContext::prepareFrame(uint32_t &imageIndex) const {
    m_vecInFlightFence[m_currentFrameIndex]->Wait();
    const auto result = m_pSwapChain->AcquireNextImage(m_vecImageAvailableSemaphore[m_currentFrameIndex], imageIndex);
    m_vecInFlightFence[m_currentFrameIndex]->Reset();
    return result;
}

void RHIContext::createSyncObject() {
    // m_vecImageAvailableSemaphore.resize(MAX_FRAME_IN_FLIGHT);
    // m_vecRenderFinishedSemaphore.resize(MAX_FRAME_IN_FLIGHT);
    // m_vecInFlightFence.resize(MAX_FRAME_IN_FLIGHT);

    for(auto i = 0; i < MAX_FRAME_IN_FLIGHT; i++) {
        m_vecImageAvailableSemaphore.emplace_back(std::make_shared<RHISemaphore>(m_pDevice));
        m_vecRenderFinishedSemaphore.emplace_back(std::make_shared<RHISemaphore>(m_pDevice));
        m_vecInFlightFence.emplace_back(std::make_shared<RHIFence>(m_pDevice));
    }
}
