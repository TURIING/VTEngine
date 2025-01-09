//
// Created by turiing on 2025/1/7.
//

#include "instance/ModelLoadInstance.h"

#include "core/pipeline/ForwardPipeLine.h"
#include "core/RHI/RHICommandBuffer.h"
#include "core/RHI/RHIDepthResource.h"
#include "core/RHI/RHISwapChain.h"
#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHIFence.h"
#include "core/RHI/RHISemaphore.h"
#include "core/RHI/Builder/RHIDescriptorPoolBuilder.h"
#include "core/RHI/Builder/RHIDescriptorSetLayoutBuilder.h"
#include "core/RHI/Builder/RHIFramebufferBuilder.h"
#include "core/RHI/Builder/RHIRenderPassBuilder.h"

constexpr int MAX_FRAME_IN_FLIGHT = 2;

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

    m_pForwardPipe = ForwardPipeLineBuilder(m_pDevice, m_pRenderPass, m_pDescriptorSetLayout)
                        .SetVertexShaderPath("common/common.vert")
                        .SetFragmentShaderPath("common/common.frag")
                        .SetPushConstantsRanges( { { VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant) } } )
                        .Build();

    for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
        m_vecFrameBuffers.emplace_back(RHIFrameBufferBuilder(m_pDevice, m_pRenderPass)
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
}
