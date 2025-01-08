//
// Created by turiing on 2025/1/7.
//

#include "instance/ModelLoadInstance.h"

#include "core/RHI/RHIDepthResource.h"
#include "core/RHI/RHISwapChain.h"
#include "core/RHI/RHIDepthResource.h"
#include "core/RHI/RHIDevice.h"
#include "core/RHI/Builder/RHIDescriptorSetLayoutBuilder.h"
#include "core/RHI/Builder/RHIRenderPassBuilder.h"

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
}
