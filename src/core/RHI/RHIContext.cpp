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
#include <core/RHI/RHIFrameBuffer.h>
#include <core/pass/ForwardPass.h>
#include <core/RHI/RHISurface.h>
#include <core/RHI/RHISwapChain.h>
#include <core/RHI/RHIInstance.h>
#include <core/RHI/RHISemaphore.h>

#include "core/RHI/RHIDescriptorPool.h"
#include "core/RHI/RHIDescriptorSet.h"
#include "core/RHI/RHIDescriptorSetLayout.h"
#include "core/RHI/RHIVertexBuffer.h"
#include "core/RHI/RHIIndexBuffer.h"
#include "core/RHI/RHIUniformBuffer.h"

constexpr int MAX_FRAME_IN_FLIGHT = 2;

RHIContext::RHIContext(const PlatformWindowInfo &info): m_size(info.size) {
    m_pInstance = std::make_shared<RHIInstance>();
    m_pSurface = std::make_shared<RHISurface>(m_pInstance, info.handle);
    m_pDevice = std::make_shared<RHIDevice>(m_pInstance, m_pSurface);
    m_pSwapChain = std::make_shared<RHISwapChain>(m_pInstance, m_pDevice, m_pSurface, info.size);
    m_pRenderPass = std::make_shared<ForwardPass>(m_pDevice, m_pSwapChain->GetColorFormat());
    std::vector<RHIDescriptorType> vecTypes = { RHIDescriptorType::ConstantBuffer };
    m_pDescriptorSetLayout = std::make_shared<RHIDescriptorSetLayout>(m_pDevice, vecTypes);
    m_pForwardPipeLine = std::make_shared<ForwardPipeLine>(m_pDevice, m_pRenderPass, m_pDescriptorSetLayout);

    const auto size = m_pSwapChain->GetSize();
    for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
        m_vecFrameBuffer.emplace_back(std::make_shared<RHIFrameBuffer>(m_pDevice, m_pRenderPass, m_pSwapChain->GetImageView(i), size));
    }

    const auto queueFamilyIndices = QueueFamilyIndices::GetQueueFamilyIndices(m_pDevice->GetPhysicalDeviceHandle(), m_pSurface->GetHandle());
    m_pCommandPool = std::make_shared<RHICommandPool>(m_pDevice, queueFamilyIndices.graphicsFamily.value());

    m_pCommandBuffer = std::make_shared<RHICommandBuffer>(m_pDevice, m_pCommandPool, MAX_FRAME_IN_FLIGHT);

    this->createSyncObject();

    m_pVertexBuffer = std::make_shared<RHIVertexBuffer>(m_pDevice, m_pCommandPool, m_vecVertices);
    m_pIndexBuffer = std::make_shared<RHIIndexBuffer>(m_pDevice, m_pCommandPool, m_vecIndices);

    m_pDescriptorPool = std::make_shared<RHIDescriptorPool>(m_pDevice);
    std::vector<VkDescriptorSetLayout> vecDescriptorSetLayouts(MAX_FRAME_IN_FLIGHT, m_pDescriptorSetLayout->GetHandle());
    m_pDescriptorSet = std::make_shared<RHIDescriptorSet>(m_pDevice, m_pDescriptorPool, vecDescriptorSetLayouts);
    for(auto i = 0; i < MAX_FRAME_IN_FLIGHT; i++) {
        m_vecUniformBuffer.emplace_back(std::make_shared<RHIUniformBuffer>(m_pDevice, sizeof(GlobalUniformObject)));
        m_pDescriptorSet->UpdateUniformBuffer(m_vecUniformBuffer[i], i, 0);
        // m_vecDescriptorSet.emplace_back(std::make_shared<RHIDescriptorSet>(m_pDevice, m_pDescriptorPool, m_pDescriptorSetLayout));
        // m_vecDescriptorSet[i]->UpdateUniformBuffer(m_vecUniformBuffer[i], 0);
    }

}

void RHIContext::Render() {
    uint32_t imageIndex = 0;
    if(!prepareFrame(imageIndex)) return;

    m_pCommandBuffer->Reset(m_currentFrameIndex);
    m_pCommandBuffer->BeginRecord(m_currentFrameIndex);
    m_pCommandBuffer->BeginRenderPass(m_pRenderPass, m_vecFrameBuffer[imageIndex], m_currentFrameIndex, m_pSwapChain->GetSize());
    m_pCommandBuffer->BindPineLine(m_pForwardPipeLine, m_currentFrameIndex);

    this->updateUniformBuffer();

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

    VkDeviceSize offset[] = { 0 };
    const std::vector<VkDescriptorSet> vecDescriptorSets(1, m_pDescriptorSet->GetHandle(m_currentFrameIndex));
    m_pCommandBuffer->SetScissor(m_currentFrameIndex, 0, 1, scissor);
    m_pCommandBuffer->BindVertexBuffer(m_currentFrameIndex, m_pVertexBuffer, offset, 0, 1);
    m_pCommandBuffer->BindIndexBuffer(m_currentFrameIndex, m_pIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    m_pCommandBuffer->BindDescriptorSets(m_currentFrameIndex, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pForwardPipeLine->GetPipelineLayout(), vecDescriptorSets, 0);
//    m_pCommandBuffer->Draw(m_currentFrameIndex, m_vecVertices.size(), 1, 0, 0);
    m_pCommandBuffer->DrawIndex(m_currentFrameIndex, m_vecIndices.size(), 1, 0, 0, 0);
    m_pCommandBuffer->EndRenderPass(m_currentFrameIndex);
    m_pCommandBuffer->EndRecord(m_currentFrameIndex);

    m_pCommandBuffer->Submit(m_currentFrameIndex, m_vecImageAvailableSemaphore[m_currentFrameIndex], m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_vecInFlightFence[m_currentFrameIndex]);
    m_pDevice->Present(m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_pSwapChain, imageIndex);

    m_currentFrameIndex = (m_currentFrameIndex + 1) % MAX_FRAME_IN_FLIGHT;
}

void RHIContext::Resize(Size size) {
    m_size = size;
    m_pDevice->WaitIdle();
    this->cleanSwapChain();

    m_pSwapChain = std::make_shared<RHISwapChain>(m_pInstance, m_pDevice, m_pSurface, size);

    const auto newSize = m_pSwapChain->GetSize();
    for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
        m_vecFrameBuffer.emplace_back(std::make_shared<RHIFrameBuffer>(m_pDevice, m_pRenderPass, m_pSwapChain->GetImageView(i), newSize));
    }
}

bool RHIContext::prepareFrame(uint32_t &imageIndex) {
    m_vecInFlightFence[m_currentFrameIndex]->Wait();

    const auto result = m_pSwapChain->AcquireNextImage(m_vecImageAvailableSemaphore[m_currentFrameIndex], imageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
         this->Resize(m_size);
        return false;
    }
    else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        LOG_CRITICAL("Failed to acquire swap chain image!");
    }

    m_vecInFlightFence[m_currentFrameIndex]->Reset();
    return true;
}

void RHIContext::createSyncObject() {
    for(auto i = 0; i < MAX_FRAME_IN_FLIGHT; i++) {
        m_vecImageAvailableSemaphore.emplace_back(std::make_shared<RHISemaphore>(m_pDevice));
        m_vecRenderFinishedSemaphore.emplace_back(std::make_shared<RHISemaphore>(m_pDevice));
        m_vecInFlightFence.emplace_back(std::make_shared<RHIFence>(m_pDevice));
    }
}

void RHIContext::cleanSwapChain() {
    m_vecFrameBuffer.clear();
    m_pSwapChain.reset();
}

void RHIContext::updateUniformBuffer() const {
    static auto startTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    const auto [width, height] = m_pSwapChain->GetSize();
    GlobalUniformObject ubo {
        .model = glm::rotate(glm::mat4(1.0f), deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .proj = glm::perspective(glm::radians(45.0f), width / static_cast<float>(height), 0.1f, 10.0f)
    };
    ubo.proj[1][1] *= -1.0f;

    m_vecUniformBuffer[m_currentFrameIndex]->UpdateBuffer(&ubo);
}
