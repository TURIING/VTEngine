//
// Created by turiing on 2025/1/4.
//
#include "core/RHI/RHISampler.h"

#include "core/RHI/RHIDevice.h"

RHISampler::RHISampler(const std::shared_ptr<RHIDevice>& device, VkFilter magFilter, VkFilter minFilter, VkSamplerAddressMode addressMode): m_pDevice(device) {
    VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = magFilter,
        .minFilter = minFilter,
        .addressModeU = addressMode,
        .addressModeV = addressMode,
        .addressModeW = addressMode,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = m_pDevice->GetPhysicalGpuInfo().limits.maxSamplerAnisotropy,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = 11.0f,
    };
    CALL_VK(vkCreateSampler(m_pDevice->GetHandle(), &samplerInfo, nullptr, &m_pSampler));
    LOG_INFO("Created sampler");
}

RHISampler::~RHISampler() {
    vkDestroySampler(m_pDevice->GetHandle(), m_pSampler, nullptr);
}
