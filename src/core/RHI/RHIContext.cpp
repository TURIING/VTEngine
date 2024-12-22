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

RHIContext::RHIContext(const PlatformWindowInfo &info) {
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

    m_pCommandBuffer = std::make_shared<RHICommandBuffer>(m_pDevice, m_pCommandPool, 1);

    m_pImageAvailableSemaphore = std::make_shared<RHISemaphore>(m_pDevice);
    m_pRenderFinishedSemaphore = std::make_shared<RHISemaphore>(m_pDevice);
    m_pInFlightFence = std::make_shared<RHIFence>(m_pDevice);
}

void RHIContext::Render() const {
    uint32_t imageIndex = 0;
    if(!prepareFrame(imageIndex)) return;

    m_pCommandBuffer->Reset(0);
    m_pCommandBuffer->BeginRecord(0);
    m_pCommandBuffer->BeginRenderPass(m_pRenderPass, m_vecFrameBuffer[imageIndex], 0, m_pSwapChain->GetSize());
    m_pCommandBuffer->BindPineLine(m_pForwardPipeLine, 0);

    constexpr VkViewport viewport = {
        .x = 0,
        .y = 0,
        .width = 1200,
        .height = 800,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    m_pCommandBuffer->SetViewport(0, 0, 1, viewport);

    constexpr VkRect2D scissor = {
        .offset = {0, 0},
        .extent = {1200, 800}
    };
    m_pCommandBuffer->SetScissor(0, 0, 1, scissor);
    m_pCommandBuffer->Draw(0);
    m_pCommandBuffer->EndRenderPass(0);
    m_pCommandBuffer->EndRecord(0);

    m_pCommandBuffer->Submit(0, m_pImageAvailableSemaphore, m_pRenderFinishedSemaphore, m_pInFlightFence);
    m_pDevice->Present(m_pRenderFinishedSemaphore, m_pSwapChain, imageIndex);
}

bool RHIContext::prepareFrame(uint32_t &imageIndex) const {
    m_pInFlightFence->Wait();
    m_pInFlightFence->Reset();
    return m_pSwapChain->AcquireNextImage(m_pImageAvailableSemaphore, imageIndex);
}