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

#include "core/common/Material.h"
#include "core/common/Mesh.h"
#include "core/Render/RenderEntity.h"
#include "core/RHI/RHIDescriptorPool.h"
#include "core/RHI/RHIDescriptorSet.h"
#include "core/RHI/RHIDescriptorSetLayout.h"
#include "core/RHI/RHIVertexBuffer.h"
#include "core/RHI/RHIIndexBuffer.h"
#include "core/RHI/RHITexture.h"
#include "core/RHI/RHIUniformBuffer.h"
#include "utility/File.h"


RHIContext::RHIContext(const PlatformWindowInfo &info): m_size(info.size) {
    std::vector<RHIDescriptorType> vecTypes = { RHIDescriptorType::ConstantBuffer, RHIDescriptorType::Sampler };
    m_pDescriptorSetLayout = std::make_shared<RHIDescriptorSetLayout>(m_pDevice, vecTypes);
    m_pForwardPipeLine = std::make_shared<ForwardPipeLine>(m_pDevice, m_pRenderPass, m_pDescriptorSetLayout);




    m_pRenderEntity = std::make_shared<RenderEntity>(m_pDevice, m_pCommandPool);

    m_pDescriptorPool = std::make_shared<RHIDescriptorPool>(m_pDevice);
    std::vector<VkDescriptorSetLayout> vecDescriptorSetLayouts(MAX_FRAME_IN_FLIGHT, m_pDescriptorSetLayout->GetHandle());
    for(auto i = 0; i < MAX_FRAME_IN_FLIGHT; i++) {
        m_vecUniformBuffer.emplace_back(std::make_shared<RHIUniformBuffer>(m_pDevice, sizeof(GlobalUniformObject)));
        m_vecDescriptorSet.emplace_back(std::make_shared<RHIDescriptorSet>(m_pDevice, m_pDescriptorPool, m_pDescriptorSetLayout));
        m_vecDescriptorSet[i]->UpdateUniformBuffer(m_vecUniformBuffer[i], 0);
        for(const auto &[bindIndex, texture] : m_pRenderEntity->GetMaterial()->GetTextures()) {
            m_vecDescriptorSet[i]->UpdateTextureImage(texture->GetDescriptorImageInfo(), bindIndex);
        }
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
    const std::vector<VkDescriptorSet> vecDescriptorSets(1, m_vecDescriptorSet[m_currentFrameIndex]->GetHandle());
    PushConstant pushConstant = { glm::mat4(1.0) };

    m_pCommandBuffer->SetScissor(m_currentFrameIndex, 0, 1, scissor);
    m_pCommandBuffer->BindVertexBuffer(m_currentFrameIndex, m_pRenderEntity->GetMesh()->GetVertexBuffer(), offset, 0, 1);
    m_pCommandBuffer->BindIndexBuffer(m_currentFrameIndex, m_pRenderEntity->GetMesh()->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
    m_pCommandBuffer->BindDescriptorSets(m_currentFrameIndex, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pForwardPipeLine->GetPipelineLayout(), vecDescriptorSets, 0);
    m_pCommandBuffer->PushConstants(m_currentFrameIndex, m_pForwardPipeLine->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pushConstant);
    m_pCommandBuffer->DrawIndex(m_currentFrameIndex, m_pRenderEntity->GetMesh()->GetIndexCount(), 1, 0, 0, 0);
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
    m_pDepthResource = std::make_shared<RHIDepthResource>(m_pDevice, m_pCommandPool, newSize);

    for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
        m_vecFrameBuffer.emplace_back(std::make_shared<RHIFrameBuffer>(m_pDevice, m_pRenderPass, m_pSwapChain->GetImageView(i), m_pDepthResource->GetImageView(), newSize));
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
}

void RHIContext::cleanSwapChain() {
    m_pDepthResource.reset();
    m_vecFrameBuffer.clear();
    m_pSwapChain.reset();
}

void RHIContext::updateUniformBuffer() const {
    const auto [width, height] = m_pSwapChain->GetSize();
    GlobalUniformObject ubo {
        .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .proj = glm::perspective(glm::radians(45.0f), width / static_cast<float>(height), 0.1f, 10.0f)
    };
    ubo.proj[1][1] *= -1.0f;

    m_vecUniformBuffer[m_currentFrameIndex]->UpdateBuffer(&ubo);
}
