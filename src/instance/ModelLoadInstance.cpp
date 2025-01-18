//
// Created by turiing on 2025/1/7.
//

#include "instance/ModelLoadInstance.h"

#include "core/common/Camera.h"
#include "core/common/Mesh.h"
#include "core/pipeline/ForwardPipeLine.h"
#include "core/Render/RenderEntity.h"
#include "core/Render/RenderScene.h"
#include "core/RHI/RHICommandBuffer.h"
#include "core/RHI/RHIDepthResource.h"
#include "core/RHI/RHIDescriptorSet.h"
#include "core/RHI/RHISwapChain.h"
#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHIFence.h"
#include "core/RHI/RHISemaphore.h"
#include "core/RHI/RHIUniformBuffer.h"
#include "core/RHI/Builder/RHIDescriptorPoolBuilder.h"
#include "core/RHI/Builder/RHIDescriptorSetLayoutBuilder.h"
#include "core/RHI/Builder/RHIFramebufferBuilder.h"
#include "core/RHI/Builder/RHIRenderPassBuilder.h"

constexpr int MAX_FRAME_IN_FLIGHT = 2;

struct PushConstant {
    alignas(16) glm::mat4 model;
};

struct GlobalUniformObject {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

ModelLoadInstance::ModelLoadInstance(const PlatformWindowInfo &info): InstanceBase(info) {
    const auto size = m_pSwapChain->GetSize();
    m_pDepthResource = std::make_shared<RHIDepthResource>(m_pDevice, m_pCommandPool, size);

    m_pRenderPass = RHIRenderPassBuilder(m_pDevice)
                        .AddColorAttachment( { 0, m_pSwapChain->GetColorFormat() })
                        .AddDepthAttachment( { 1, m_pDevice->GetDepthFormatDetail() } )
                        .Build();

    m_pDescriptorSetLayout = RHIDescriptorSetLayoutBuilder(m_pDevice)
                                .AddDescriptorBinding( { 0, RHIDescriptorType::ConstantBuffer } )
                                .AddDescriptorBinding( { 1, RHIDescriptorType::Sampler } )
                                .Build();

    m_pForwardPipeLine = ForwardPipeLineBuilder(m_pDevice, m_pRenderPass, m_pDescriptorSetLayout)
                        .SetVertexShaderPath("common/vert.spv")
                        .SetFragmentShaderPath("common/frag.spv")
                        .SetPushConstantsRanges( { { VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant) } } )
                        .Build();

    for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
        m_vecFrameBuffer.emplace_back(RHIFrameBufferBuilder(m_pDevice, m_pRenderPass)
                                            .AddAttachmentImageView(m_pSwapChain->GetImageView(i))
                                            .AddAttachmentImageView(m_pDepthResource->GetImageView())
                                            .SetSize(size)
                                            .Build());
    }

    m_pCommandBuffer = std::make_shared<RHICommandBuffer>(m_pDevice, m_pCommandPool, MAX_FRAME_IN_FLIGHT);

    for(auto i = 0; i < MAX_FRAME_IN_FLIGHT; i++) {
        m_vecImageAvailableSemaphore.emplace_back(std::make_shared<RHISemaphore>(m_pDevice));
        m_vecRenderFinishedSemaphore.emplace_back(std::make_shared<RHISemaphore>(m_pDevice));
        m_vecInFlightFence.emplace_back(std::make_shared<RHIFence>(m_pDevice));
    }

    m_pDescriptorPool = RHIDescriptorPoolBuilder(m_pDevice)
                            .AddPoolSize( { RHIDescriptorType::Sampler, 1000 } )
                            .AddPoolSize( { RHIDescriptorType::ConstantBuffer, 1000 } )
                            .Build();

    m_pRenderScene->AddRenderEntity(MODEL_DIR + "viking room/viking_room.obj");

    std::vector<VkDescriptorSetLayout> vecDescriptorSetLayouts(MAX_FRAME_IN_FLIGHT, m_pDescriptorSetLayout->GetHandle());
    for(auto i = 0; i < MAX_FRAME_IN_FLIGHT; i++) {
        m_vecUniformBuffer.emplace_back(std::make_shared<RHIUniformBuffer>(m_pDevice, sizeof(GlobalUniformObject)));
        m_vecDescriptorSet.emplace_back(std::make_shared<RHIDescriptorSet>(m_pDevice, m_pDescriptorPool, m_pDescriptorSetLayout));
        m_vecDescriptorSet[i]->UpdateUniformBuffer(m_vecUniformBuffer[i], 0);
        m_pRenderScene->UpdateEntityTexture(m_vecDescriptorSet[i]);
    }
}

void ModelLoadInstance::OnRender() {
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
    m_pCommandBuffer->BindVertexBuffer(m_currentFrameIndex, m_pRenderScene->GetEntity()->GetMesh()->GetVertexBuffer(), offset, 0, 1);
    m_pCommandBuffer->BindIndexBuffer(m_currentFrameIndex, m_pRenderScene->GetEntity()->GetMesh()->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
    m_pCommandBuffer->BindDescriptorSets(m_currentFrameIndex, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pForwardPipeLine->GetPipelineLayout(), vecDescriptorSets, 0);
    m_pCommandBuffer->PushConstants(m_currentFrameIndex, m_pForwardPipeLine->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pushConstant);
    m_pCommandBuffer->DrawIndex(m_currentFrameIndex, m_pRenderScene->GetEntity()->GetMesh()->GetIndexCount(), 1, 0, 0, 0);
    m_pCommandBuffer->EndRenderPass(m_currentFrameIndex);
    m_pCommandBuffer->EndRecord(m_currentFrameIndex);

    m_pCommandBuffer->Submit(m_currentFrameIndex, m_vecImageAvailableSemaphore[m_currentFrameIndex], m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_vecInFlightFence[m_currentFrameIndex]);
    m_pDevice->Present(m_vecRenderFinishedSemaphore[m_currentFrameIndex], m_pSwapChain, imageIndex);

    m_currentFrameIndex = (m_currentFrameIndex + 1) % MAX_FRAME_IN_FLIGHT;
}

bool ModelLoadInstance::prepareFrame(uint32_t &imageIndex) {
    m_vecInFlightFence[m_currentFrameIndex]->Wait();

    const auto result = m_pSwapChain->AcquireNextImage(m_vecImageAvailableSemaphore[m_currentFrameIndex], imageIndex);
    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
         this->OnResize(m_size);
        return false;
    }
    else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        LOG_CRITICAL("Failed to acquire swap chain image!");
    }

    m_vecInFlightFence[m_currentFrameIndex]->Reset();
    return true;
}

void ModelLoadInstance::updateUniformBuffer() const {
    const auto camera = m_pRenderScene->GetCamera();
    GlobalUniformObject ubo {
        .view = camera->GetViewMatrix(),
        .proj = camera->GetProjection()
    };
    ubo.proj[1][1] *= -1.0f;

    m_vecUniformBuffer[m_currentFrameIndex]->UpdateBuffer(&ubo);
}

void ModelLoadInstance::cleanSwapChain() {
    m_pDepthResource.reset();
    m_vecFrameBuffer.clear();
    m_pSwapChain.reset();
}
